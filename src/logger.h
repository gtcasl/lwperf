#pragma once

#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <vector>

namespace lwperf {

struct NameValuePairs {
  void addPair(std::string name, double value) {
    names.emplace_back(name);
    values.emplace_back(value);
  }
  std::vector<std::string> names;
  std::vector<double> values;
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
        auto new_cite = Cite{cite_name};
        auto insert = cites_.emplace(cite_name, std::move(new_cite));
        cite = insert.first;
      }
      cite->second.addDetMetric(param_name, value);
    }
    void log(const char* cite_name) {
      auto& cite = cites_[cite_name];
      cite.start_time = std::chrono::high_resolution_clock::now();
    }
    void stop(const char* cite_name) {
      auto end_time = std::chrono::high_resolution_clock::now();
      auto& cite = cites_[cite_name];
      auto elapsed_time =
          std::chrono::duration<double>(end_time - cite.start_time).count();
      std::vector<std::string> result_names = {"time"};
      std::vector<double> result_values = {elapsed_time};
      cite.commit(invariants_.names, invariants_.values, result_names, result_values);
    }
  private:
    class Cite {
     private:
      Backend backend;
      NameValuePairs det_metrics_;
      bool headers_committed;
     public:
      Cite() {}
      Cite(const char* name) : backend{name}, headers_committed{false} {}
      void addDetMetric(const std::string& name, double value) {
        /* only add a new pair if the metric doesn't exist. otherwise update 
         * the registered value. */
        auto metric =
            std::find(begin(det_metrics_.names), end(det_metrics_.names), name);
        if(metric == end(det_metrics_.names)){
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
        if(!headers_committed){
          backend.commit_headers(invariant_names, det_metrics_.names, result_names);
          headers_committed = true;
        }
        backend.commit_values(invariant_values, det_metrics_.values, result_values);
      }
      std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    };

    std::unordered_map<std::string, Cite> cites_;
    NameValuePairs invariants_;
};

} // end namespace lwperf
