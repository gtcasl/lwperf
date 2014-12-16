#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>

#include <eiger.h>

namespace lwperf {

class EigerBackend{
  public:
    EigerBackend() {}
    EigerBackend(const char* cite_name);
    ~EigerBackend();
    static void register_configuration(const char* machine, const char* application,
                                       const char* dbname, const char* prefix,
                                       const char* suffix);
    void commit_headers(const std::vector<std::string>& invariant_names,
                        const std::vector<std::string>& parameter_names,
                        const std::vector<std::string>& result_names);
    void commit_values(const std::vector<double>& invariant_values,
                       const std::vector<double>& parameter_values,
                       const std::vector<double>& result_values);
  private:
    static eiger::MachineID machine_id_;
    static eiger::ApplicationID app_id_;
    static std::string app_name_;
    eiger::DataCollectionID dc_id_;
    std::vector<eiger::MetricID> invariant_metrics_;
    std::vector<std::string> invariant_names_;
    std::vector<eiger::MetricID> parameter_metrics_;
    std::vector<std::string> parameter_names_;
    std::vector<eiger::MetricID> result_metrics_;
};

} // end namespace lwperf
