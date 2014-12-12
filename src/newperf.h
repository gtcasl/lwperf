#pragma once

#include <string>
#include <tuple>
#include <functional>
#include <mutex>

namespace lwperf {

class Logger {
  public:
    Logger(std::string machine, std::string app, std::string dbname,
           std::string prefix, std::string suffix);
    ~Logger(); 
  private:
};

void init(std::string machine, std::string app, std::string dbname,
          std::string prefix, std::string suffix);
void finalize();

template<typename... Targs>
void dump_to_backend(std::string cite_name, Targs... args) {
}

template<typename... Targs>
class log{
  public:
    log(std::string cite_name, Targs... rest) {
      dump_fn_ = std::bind(dump_to_backend, cite_name, rest...);
      clock_gettime(CLOCK_MONOTONIC, &start_time_);
    }
    ~log() {
      timespec stop_time;
      clock_gettime(CLOCK_MONOTONIC, &stop_time);
      auto elapsed = get_elapsed_time(start_time_, stop_time);
      {
        std::lock_guard<std::mutex> lock(global_mutex_);
        dump_fn_();
      }
    }
  private:
    std::function<void(void)> dump_fn_;
    timespec start_time_;
    static std::mutex global_mutex_;
};

} // end namepace lwperf
