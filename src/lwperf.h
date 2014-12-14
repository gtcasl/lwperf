#pragma once

#include "lwperf_impl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LWPERF_IMPL* lwperf_t;

inline lwperf_t lwperf_init(const char* machine, const char* application,
                            const char* dbname, const char* prefix,
                            const char* suffix) {
  return lwperf_init_impl(machine, application, dbname, prefix, suffix);
}
inline void lwperf_finalize(lwperf_t perf) { lwperf_finalize_impl(perf); }
inline void lwperf_add_invariant(lwperf_t perf, const char* name,
                                 double value) {
  lwperf_add_invariant_impl(perf, name, value);
}
inline void lwperf_add_cite_param(lwperf_t perf, const char* cite_name,
                                  const char* param_name, double value) {
  lwperf_add_cite_param_impl(perf, cite_name, param_name, value);
}
inline void lwperf_log(lwperf_t perf, const char* cite_name) {
  lwperf_log_impl(perf, cite_name);
}
inline void lwperf_stop(lwperf_t perf, const char* cite_name) {
  lwperf_stop_impl(perf, cite_name);
}
/* 
 * This function can take a variable number of name--value pairs as arguments.
 * For example:
 * lwperf_add_cite_params(perf, cite_name, name_1, value_1, name_2, value_2, ...)
 * Where the parameter types are
 *    const char *name_1, *name_2;
 *    double value_1, value_2;
 */
#define lwperf_add_cite_params(perf, cite_name, ...) \
  lwperf_add_cite_params_impl(perf, cite_name, __VA_ARGS__)

#ifdef __cplusplus
} // extern "C"
#endif
