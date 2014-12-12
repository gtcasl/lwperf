#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>

#include <eiger.h>

class Cite {
  private:
    eiger::DataCollection dc_;
    std::unordered_map<std::string, double> det_metrics_;
  public:
    Cite(const char* name, const std::unordered_map<std::string, double>& invariants);
    void addDetMetric(const std::string& name, double value);
    const std::unordered_map<std::string, double>& getDetMetrics() const;
    eiger::DataCollectionID getDCID();
    eiger::TrialID trial_id;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};

class lwperf_eiger{
  private:
    eiger::Machine machine_;
    eiger::Application app_;
    eiger::MetricID exec_time_id_;
    std::unordered_map<std::string, Cite> cites_;
    std::unordered_map<std::string, double> invariants_;
    std::unordered_map<std::string, eiger::MetricID> metric_ids_;

  public:
    void add_invariant(const char* name, double value);
    void add_cite_param(const char* cite_name, const char* param_name, double value);
    void log(const char* cite_name);
    void stop(const char* cite_name);

    lwperf_eiger(const char* machine, const char* application, const char* dbname,
                 const char* prefix, const char* suffix);
    ~lwperf_eiger();
};
