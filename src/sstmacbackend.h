#pragma once

#include <string>
#include <map>
#include <vector>

namespace lwperf {

class SSTMACBackend{
  private:
    std::string model_file_;
    std::map<std::string, double> params_;
    std::vector<std::string> invariant_names_, parameter_names_, result_names_;

  public:
    SSTMACBackend(const char* site_name);
    static void register_configuration(const char* machine, const char* application,
                                       const char* dbname) {
      (void)machine;
      (void)application;
      (void)dbname;
    }
    void commit_headers(const std::vector<std::string>& invariant_names,
                        const std::vector<std::string>& parameter_names,
                        const std::vector<std::string>& result_names);
    void commit_values(const std::vector<double>& invariant_values,
                       const std::vector<double>& parameter_values,
                       const std::vector<double>& result_values);
};

} // end namespace lwperf
