#include <vector>
#include <fstream>
#include <sstream>

#include "csvbackend.h"

namespace lwperf {

template<typename T>
void print_comma_separated(std::ofstream& out, const std::vector<T>& things) {
  const auto& last = things.back();
  for(const auto& elem : things) {
    if(&elem != &last){
      out << elem << ",";
    } else {
      out << elem;
    }
  }
}

template<typename T>
void print_comma_separated(std::ofstream& out, const std::vector<T>& first,
                           const std::vector<T>& second,
                           const std::vector<T>& third){
  print_comma_separated(out, first);
  if(!first.empty() && (!second.empty() || !third.empty())){
    out << ",";
  }
  print_comma_separated(out, second);
  if(!second.empty() && !third.empty()){
    out << ",";
  }
  print_comma_separated(out, third);
  out << "\n";
}

void CSVBackend::commit_headers(const std::vector<std::string>& invariant_names,
                                const std::vector<std::string>& parameter_names,
                                const std::vector<std::string>& result_names) {
  std::stringstream file_ss;
  file_ss << site_name_ << ".csv";
  std::ofstream ofile(file_ss.str(), std::ios::app);
  // only write headers if output file is empty
  if(ofile.tellp() == 0){
    print_comma_separated(ofile, invariant_names, parameter_names,
                          result_names);
  }
}

void CSVBackend::commit_values(
    const std::vector<double>& invariant_values,
    const std::vector<double>& parameter_values,
    const std::vector<double>& result_values) {
  std::stringstream file_ss;
  file_ss << site_name_ << ".csv";
  std::ofstream ofile(file_ss.str(), std::ios::app);
  print_comma_separated(ofile, invariant_values, parameter_values,
                        result_values);
}

} // end namespace lwperf

