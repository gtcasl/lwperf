#pragma once

#include "lwperf_impl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LWPERF_IMPL* lwperf_t;


inline lwperf_t lwperf_init(const char* machine, const char* application,
                            const char* dbname) {
  return lwperf_init_impl(machine, application, dbname);
}

inline void lwperf_finalize(lwperf_t perf) { lwperf_finalize_impl(perf); }

inline void lwperf_add_invariant(lwperf_t perf, const char* name,
                                 double value) {
  lwperf_add_invariant_impl(perf, name, value);
}

inline void lwperf_add_site_param(lwperf_t perf, const char* site_name,
                                  const char* param_name, double value) {
  lwperf_add_site_param_impl(perf, site_name, param_name, value);
}

inline void lwperf_log(lwperf_t perf, const char* site_name) {
  lwperf_log_impl(perf, site_name);
}

inline void lwperf_stop(lwperf_t perf, const char* site_name) {
  lwperf_stop_impl(perf, site_name);
}

inline void lwperf_init_papi(lwperf_t perf) {
  lwperf_init_papi_impl(perf);
}
/* 
 * This function can take a variable number of name--value pairs as arguments.
 * For example:
 * lwperf_add_site_params(perf, site_name, name_1, value_1, name_2, value_2, ...)
 * Where the parameter types are
 *    const char *name_1, *name_2;
 *    double value_1, value_2;
 */
#define lwperf_add_site_params(perf, site_name, ...) \
  lwperf_add_site_params_impl(perf, site_name, __VA_ARGS__)

#ifdef __cplusplus
} // extern "C"
#endif
