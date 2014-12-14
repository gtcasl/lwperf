#include <sstream>
#include <map>
#include <cassert>
#include <eiger.h>

#include "eigerbackend.h"

Cite::Cite(const char* name, const std::unordered_map<std::string, double>& invariants) 
  : dc_(name, name), det_metrics_(invariants) {}

void Cite::addDetMetric(const std::string& name, double value) {
  det_metrics_[name] = value;
}

const std::unordered_map<std::string, double>& Cite::getDetMetrics() const {
  return det_metrics_;
}

eiger::DataCollectionID Cite::getDCID() {
  return dc_.getID();
}

EigerBackend::EigerBackend(const char* machine, const char* application,
                           const char* dbname, const char* prefix,
                           const char* suffix)
    : machine_(machine, machine),
      app_(application, application)
{
  (void)prefix;
  (void)suffix;
  eiger::Connect(dbname);
  machine_.commit();
  app_.commit();
  eiger::Metric exec(eiger::NONDETERMINISTIC, "time", "execution time in seconds");
  exec.commit();
  exec_time_id_ = exec.getID();
}


EigerBackend::~EigerBackend() {
  eiger::Disconnect();
}

void EigerBackend::add_invariant(const char* name, double value) {
  invariants_.emplace(name, value);
  eiger::Metric metric(eiger::DETERMINISTIC, name, name);
  metric.commit();
  metric_ids_[name] = metric.getID();
}

void EigerBackend::add_cite_param(const char* cite_name, const char* param_name,
                                  double value) {
  eiger::Metric metric(eiger::DETERMINISTIC, param_name, param_name);
  metric.commit();
  metric_ids_[param_name] = metric.getID();
  auto cite = cites_.find(cite_name);
  if(cite == end(cites_)){ // Need to make a new Cite
    auto new_cite = Cite{cite_name, invariants_};
    auto insert = cites_.emplace(cite_name, std::move(new_cite));
    cite = insert.first;
  }
  cite->second.addDetMetric(param_name, value);
}

void EigerBackend::log(const char* cite_name) {
  auto& cite = cites_[cite_name];
  // new dataset for this dc
  std::ostringstream dset_name; 
  dset_name << app_.name;
  for(const auto& det_metric : cite.getDetMetrics()){
    dset_name << "_" << det_metric.first << "_" << det_metric.second;
  } 
  eiger::Dataset dset(app_.getID(), dset_name.str(), dset_name.str(), "");
  dset.commit();
  eiger::DatasetID dsid = dset.getID();
  // commit our saved values
  for(const auto& det_metric : cite.getDetMetrics()){
    eiger::DeterministicMetric metric(dsid, metric_ids_[det_metric.first],
                                      det_metric.second);
    metric.commit();
  }
  // new trial
  eiger::Trial trial(cite.getDCID(), machine_.getID(), app_.getID(), dsid);
  // save trialID to the Cite
  cite.trial_id = trial.getID();
  // start timer
  cite.start_time = std::chrono::high_resolution_clock::now();
}

void EigerBackend::stop(const char* cite_name) {
  // stop timer
  auto end_time = std::chrono::high_resolution_clock::now();
  auto& cite = cites_[cite_name];
  auto elapsed_time = std::chrono::duration<double>(end_time - cite.start_time);
  // create new nondetmetric for exec time and commit
  eiger::NondeterministicMetric metric(cite.trial_id, exec_time_id_, elapsed_time);
  metric.commit();
}

