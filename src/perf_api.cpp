#include <cassert>

#include "logger.h"
#include "backend.h"
#include "null_backend.h"

#include "perf_api.h"

extern "C" {

/*******    API    ********/
lwperf_null* lwperf_init(const char* machine, const char* application,
                          const char* dbname, const char* prefix,
                          const char* suffix) {
  (void)machine;
  (void)application;
  (void)dbname;
  (void)prefix;
  (void)suffix;
  return new lwperf_null();
}

void lwperf_finalize(lwperf_t perf) {
  delete perf;
}

void lwperf_add_invariant(lwperf_t perf, const char* name, double value) {
  perf->add_invariant(name, value);
}

void lwperf_add_cite_param(lwperf_t perf, const char* cite_name,
                           const char* param_name, double value) {
  perf->add_cite_param(cite_name, param_name, value);
}

void lwperf_log(lwperf_t perf, const char* cite_name) {
  perf->log(cite_name);
}

void lwperf_stop(lwperf_t perf, const char* cite_name) {
  perf->stop(cite_name);
}

}

