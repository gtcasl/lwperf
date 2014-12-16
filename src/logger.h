#pragma once

#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <vector>
#include <string>
#include <iostream>

#ifdef USE_PAPI
#include <papi.h>
#endif

namespace {
const std::string kEventName = "rapl:::PACKAGE_ENERGY:PACKAGE0";
}

namespace lwperf {

struct NameValuePairs {
  void addPair(std::string name, double value) {
    names.emplace_back(name);
    values.emplace_back(value);
  }
  std::vector<std::string> names;
  std::vector<double> values;
};

template<typename Backend>
class Cite {
 private:
  Backend backend;
  NameValuePairs det_metrics_;
  bool headers_committed;

 public:
  Cite() {}
  Cite(const char* name) : backend{name}, headers_committed{false} 
#ifdef USE_PAPI
      , eventset{PAPI_NULL} 
  {
    int retval;
    if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT){
      std::cerr << "Unable to init PAPI library." << std::endl;
      exit(-1);
    }
    int event_code;
    retval = PAPI_event_name_to_code(const_cast<char*>(kEventName.c_str()),
                                     &event_code);
    if(retval != PAPI_OK){
      PAPI_perror(NULL);
      exit(-1);
    }
    retval = PAPI_create_eventset(&eventset);
    if(retval != PAPI_OK){
      PAPI_perror(NULL);
      exit(-1);
    }
    retval = PAPI_add_event(eventset, event_code);
    if(retval != PAPI_OK){
      PAPI_perror(NULL);
      exit(-1);
    }
  }
#else
  {}
#endif
  void addDetMetric(const std::string& name, double value) {
    /* only add a new pair if the metric doesn't exist. otherwise update
     * the registered value. */
    auto metric =
        std::find(begin(det_metrics_.names), end(det_metrics_.names), name);
    if (metric == end(det_metrics_.names)) {
      det_metrics_.addPair(name, value);
    } else {
      auto idx = std::distance(begin(det_metrics_.names), metric);
      det_metrics_.values[idx] = value;
    }
  }
  void commit(const std::vector<std::string>& invariant_names,
              const std::vector<double>& invariant_values,
              const std::vector<std::string>& result_names,
              const std::vector<double>& result_values) {
    if (!headers_committed) {
      backend.commit_headers(invariant_names, det_metrics_.names, result_names);
      headers_committed = true;
    }
    backend.commit_values(invariant_values, det_metrics_.values, result_values);
  }

  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  int eventset;
};

template <typename Backend>
class Logger {
  public:
    Logger(const char* machine, const char* application, const char* dbname,
           const char* prefix, const char* suffix) {
      Backend::register_configuration(machine, application, dbname, prefix,
                                      suffix);
    }
    ~Logger();
    void add_invariant(const char* name, double value) {
      invariants_.addPair(name, value);
    }
    void add_cite_param(const char* cite_name, const char* param_name,
                        double value) {
      auto cite = cites_.find(cite_name);
      if(cite == end(cites_)){ // Need to make a new Cite
        auto new_cite = Cite<Backend>{cite_name};
        auto insert = cites_.emplace(cite_name, std::move(new_cite));
        cite = insert.first;
      }
      cite->second.addDetMetric(param_name, value);
    }
    void log(const char* cite_name) {
      auto& cite = cites_[cite_name];
#ifdef USE_PAPI
      int retval = PAPI_start(cite.eventset);
      if(retval != PAPI_OK){
        PAPI_perror(NULL);
        exit(-1);
      }
#endif
      cite.start_time = std::chrono::high_resolution_clock::now();
    }
    void stop(const char* cite_name) {
      auto end_time = std::chrono::high_resolution_clock::now();
      auto& cite = cites_[cite_name];
#ifdef USE_PAPI
      long long elapsed_energy;
      int retval = PAPI_stop(cite.eventset, &elapsed_energy);
      if(retval != PAPI_OK){
        PAPI_perror(NULL);
        exit(-1);
      }
#endif
      auto elapsed_time =
          std::chrono::duration<double>(end_time - cite.start_time).count();
      std::vector<std::string> result_names = {"time"};
      std::vector<double> result_values = {elapsed_time};
#ifdef USE_PAPI
      result_names.emplace_back(kEventName);
      result_values.emplace_back(elapsed_energy);
#endif
      cite.commit(invariants_.names, invariants_.values, result_names, result_values);
    }
  private:
    std::unordered_map<std::string, Cite<Backend>> cites_;
    NameValuePairs invariants_;
};

} // end namespace lwperf
