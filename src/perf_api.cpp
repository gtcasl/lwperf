#include <cassert>

#include "logger.h"
#include "backend.h"
#include "null_backend.h"

#include "perf_api.h"

extern "C" {

/*******    API    ********/
lwperf_t lwperf_init(const char* machine, const char* application,
                     const char* dbname, const char* prefix,
                     const char* suffix) {
#if defined(_USE_EIGER)
  return new lwperf_eiger();
#elif defined(_USE_FAKEEIGER)
  return new lwperf_fake();
#elif defined(_USE_EIGER_MODEL)
  return new lwperf_model();
#else
  return new lwperf_null();
#endif
}

void lwperf_finalize(lwperf_t perf) {
  delete perf;
}

void lwperf_stop(lwperf_t perf, const char* cite_name) {
  perf->stop(cite_name);
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

}

