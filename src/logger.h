#pragma once
#include "perf_api.h"

namespace lwperf {

class Logger {
  public:
  Logger(lwperf_backend_t backend, const char* machine, const char* application,
         const char* dbname, const char* prefix, const char* suffix);

  void stop(const char* cite_name);
};

} // end namespace lwperf
