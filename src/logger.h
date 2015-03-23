#pragma once

#include "lwperf_config.h"
#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <vector>
#include <string>
#include <iostream>
#include <memory>

#ifdef LWPERF_HAVE_PAPI
#include <papi.h>
#include "papi-helpers.hpp"
using namespace papi;

namespace {
const std::string kDefaultPAPIEventName = "rapl:::PACKAGE_ENERGY:PACKAGE0";
}
#endif

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
class Site {
 private:
  Backend backend;
  NameValuePairs det_metrics_;
  bool headers_committed;

 public:
  Site() {}
  Site(const char* name) : backend{name}, headers_committed{false} {}
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
#ifdef LWPERF_HAVE_PAPI
  std::vector<int> eventsets;
#endif
};

template <typename Backend>
class Logger {
  public:
    Logger(const char* machine, const char* application, const char* dbname) 
        : do_measurement_{false}, do_papi_{false} {
      Backend::register_configuration(machine, application, dbname);
    }

    void add_invariant(const char* name, double value) {
      auto variant = std::find(begin(invariants_.names), end(invariants_.names), name);
      if(variant != end(invariants_.names)){
        auto idx = std::distance(begin(invariants_.names), variant);
        invariants_.values[idx] = value;
      } else {
        invariants_.addPair(name, value);
      }
    }

    Site<Backend>& get_site(const char* site_name) {
      auto site = sites_.find(site_name);
      if(site == end(sites_)){ // Need to make a new Site
        std::unique_ptr<Site<Backend>> new_site{new Site<Backend>(site_name)};
        auto insert = sites_.emplace(site_name, std::move(new_site));
        site = insert.first;
#ifdef LWPERF_HAVE_PAPI
        if(do_papi_){
          site->second->eventsets = make_event_sets(papi_event_infos_);
        }
#endif
      }
      return *site->second;
    }

    void add_site_param(const char* site_name, const char* param_name,
                        double value) {
      auto& site = get_site(site_name);
      site.addDetMetric(param_name, value);
    }

    void log(const char* site_name) {
      if(do_measurement_) {
        auto& site = get_site(site_name);
#ifdef LWPERF_HAVE_PAPI
        if(do_papi_){
          for(const auto eventset : site.eventsets){
            int retval = PAPI_start(eventset);
            if(retval != PAPI_OK){
              PAPI_perror(NULL);
              exit(-1);
            }
          }
        }
#endif
        site.start_time = std::chrono::high_resolution_clock::now();
      }
    }

    void stop(const char* site_name) {
      auto& site = get_site(site_name);
      std::vector<std::string> result_names;
      std::vector<double> result_values;
      if(do_measurement_) {
        auto end_time = std::chrono::high_resolution_clock::now();
#ifdef LWPERF_HAVE_PAPI
        if(do_papi_){
          for(std::size_t i = 0; i < papi_event_infos_.size(); ++i){
            std::vector<long long> values(papi_event_infos_[i].codes.size());
            int retval = PAPI_stop(site.eventsets[i], &values[0]);
            if(retval != PAPI_OK){
              PAPI_perror(NULL);
              exit(-1);
            }
            result_values.insert(end(result_values), begin(values), end(values));
          }
        }
#endif
        auto elapsed_time =
            std::chrono::duration<double>(end_time - site.start_time).count();
        result_values.emplace_back(elapsed_time);
      }
#ifdef LWPERF_HAVE_PAPI
      if(do_papi_){
        for(std::size_t i = 0; i < papi_event_infos_.size(); ++i){
          result_names.insert(end(result_names), begin(papi_event_infos_[i].names),
                              end(papi_event_infos_[i].names));
        }
      }
#endif
      result_names.emplace_back("time");
      site.commit(invariants_.names, invariants_.values, result_names, result_values);
    }

    void init_papi(const char** counter_names, int num_counter_names) {
#ifdef LWPERF_HAVE_PAPI
      if(!do_papi_){
        int retval;
        if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT){
          std::cerr << "Unable to init PAPI library." << std::endl;
          exit(-1);
        }
        papi_event_infos_ = init_papi_counters(counter_names, num_counter_names);
      }
      do_papi_ = true;
#endif
    }

    void init_papi() {
#ifdef LWPERF_HAVE_PAPI
      const char* counter_names[1];
      counter_names[0] = kDefaultPAPIEventName.c_str();
      init_papi(counter_names, 1);
#endif
    }

    void enable_measurement() {
      do_measurement_ = true;
    }

  private:
    std::unordered_map<std::string, std::unique_ptr<Site<Backend>>> sites_;
    NameValuePairs invariants_;
    bool do_measurement_;
    bool do_papi_;
#ifdef LWPERF_HAVE_PAPI
    std::vector<event_info_t> papi_event_infos_;
#endif
};

} // end namespace lwperf
