#include <sstream>
#include <map>
#include <vector>
#include <sstmac/software/api/eiger/sstmac_compute_eiger.h>

#include "sstmacbackend.h"

namespace lwperf {

SSTMACBackend::SSTMACBackend(const char* site_name) {
  std::stringstream model_ss{site_name};
  model_ss << ".model";
  model_file_ = model_ss.str();
}


void SSTMACBackend::commit_headers(const std::vector<std::string>& invariant_names,
                                   const std::vector<std::string>& parameter_names,
                                   const std::vector<std::string>& result_names) {
  invariant_names_ = invariant_names;
  parameter_names_ = parameter_names;
  result_names_ = result_names;
}

void SSTMACBackend::commit_values(
      const std::vector<double>& invariant_values,
      const std::vector<double>& parameter_values,
      const std::vector<double>& result_values) {
  for(unsigned i = 0; i < invariant_names_.size(); ++i){
    params_[invariant_names_[i]] = invariant_values[i];
  }
  for(unsigned i = 0; i < parameter_names_.size(); ++i){
    params_[parameter_names_[i]] = parameter_values[i];
  }
  SSTMAC_compute_eiger(params_, model_file_);
}

} // end namespace lwperf

