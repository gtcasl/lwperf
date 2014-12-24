#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <memory>

namespace lwperf {

class CSVBackend{
  private:
    std::unique_ptr<std::ofstream> output_file_;

  public:
    CSVBackend(const char* site_name);
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
