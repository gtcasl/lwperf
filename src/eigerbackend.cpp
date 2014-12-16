#include <sstream>
#include <map>
#include <cassert>
#include <eiger.h>

#include "eigerbackend.h"

namespace lwperf {

void EigerBackend::register_configuration(const char* machine,
                                          const char* application,
                                          const char* dbname,
                                          const char* prefix,
                                          const char* suffix) {
  (void)prefix;
  (void)suffix;
  eiger::Connect(dbname);
  eiger::Machine e_machine(machine, machine);
  e_machine.commit();
  EigerBackend::machine_id_ = e_machine.getID();
  eiger::Application e_app(application, application);
  e_app.commit();
  EigerBackend::app_id_ = e_app.getID();
  app_name_ = application;
}

EigerBackend::EigerBackend(const char* cite_name) {
  eiger::DataCollection dc(cite_name, cite_name);
  dc.commit();
  dc_id_ = dc.getID();
}

EigerBackend::~EigerBackend() {
  eiger::Disconnect();
}

void EigerBackend::commit_headers(
    const std::vector<std::string>& invariant_names,
    const std::vector<std::string>& parameter_names,
    const std::vector<std::string>& result_names) {
  // precondition: this only gets called once
  invariant_names_ = invariant_names;
  parameter_names_ = parameter_names;
  for(const auto& invariant : invariant_names){
    eiger::Metric metric(eiger::DETERMINISTIC, invariant, invariant);
    metric.commit();
    invariant_metrics_.emplace_back(metric.getID());
  }
  for(const auto& parameter : parameter_names){
    eiger::Metric metric(eiger::DETERMINISTIC, parameter, parameter);
    metric.commit();
    parameter_metrics_.emplace_back(metric.getID());
  }
  for(const auto& result : result_names){
    eiger::Metric metric(eiger::NONDETERMINISTIC, result, result);
    metric.commit();
    result_metrics_.emplace_back(metric.getID());
  }
}

void EigerBackend::commit_values(
    const std::vector<double>& invariant_values,
    const std::vector<double>& parameter_values,
    const std::vector<double>& result_values) {
  std::ostringstream dset_name; 
  dset_name << EigerBackend::app_name_;
  for(const auto& i_name : invariant_names_){
    dset_name << "_" << i_name;
  }
  for(const auto& i_value : invariant_values){
    dset_name << "_" << i_value;
  }
  for(const auto& p_name : parameter_names_){
    dset_name << "_" << p_name;
  }
  for(const auto& p_value : parameter_values){
    dset_name << "_" << p_value;
  }
  eiger::Dataset dset(EigerBackend::app_id_, dset_name.str(), dset_name.str(), "");
  dset.commit();
  eiger::DatasetID dsid = dset.getID();
  for(unsigned int invariant_idx = 0; invariant_idx < invariant_values.size();
      ++invariant_idx){
    eiger::DeterministicMetric metric(dsid, invariant_metrics_[invariant_idx],
                                      invariant_values[invariant_idx]);
    metric.commit();
  }
  for(unsigned int param_idx = 0; param_idx < parameter_values.size();
      ++param_idx){
    eiger::DeterministicMetric metric(dsid, parameter_metrics_[param_idx],
                                      parameter_values[param_idx]);
    metric.commit();
  }
  eiger::Trial trial(dc_id_, EigerBackend::machine_id_, EigerBackend::app_id_,
                     dsid);
  trial.commit();
  auto t_id = trial.getID();
  for(unsigned int result_idx = 0; result_idx < result_values.size();
      ++result_idx){
    eiger::NondeterministicMetric metric(t_id, result_metrics_[result_idx],
                                         result_values[result_idx]);
    metric.commit();
  }
}

eiger::MachineID EigerBackend::machine_id_;
eiger::ApplicationID EigerBackend::app_id_;
std::string EigerBackend::app_name_;

} // end namespace lwperf

