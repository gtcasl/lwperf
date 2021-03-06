#pragma once

#include "lwperf_config.h"

/*
 * NOTE: This header contains implementation details and are subject to change.
 * Use the public API.
 */

#ifdef __cplusplus
extern "C" {
#endif

#define LWPERF_PRIMITIVE_CAT(x, y) x ## y
#define LWPERF_CAT(x,y) LWPERF_PRIMITIVE_CAT(x,y)

#if defined(LWPERF_USE_EIGER)
#ifndef LWPERF_HAVE_EIGER
#error "Eiger support was not compiled into this library"
#endif
#define LWPERF_IMPL_BASE eiger
#elif defined(LWPERF_USE_CSV)
#define LWPERF_IMPL_BASE csv
#elif defined(LWPERF_USE_SSTMAC)
#ifndef LWPERF_HAVE_SSTMAC
#error "SST/macro support was not compiled into this library"
#endif
#define LWPERF_IMPL_BASE sstmac
#else
#define LWPERF_IMPL_BASE null
#endif

#define LWPERF_IMPL LWPERF_CAT(lwperf_, LWPERF_IMPL_BASE)

typedef struct LWPERF_IMPL* lwperf_t;
#define lwperf_init_impl LWPERF_CAT(lwperf_init_, LWPERF_IMPL_BASE)
#define lwperf_finalize_impl LWPERF_CAT(lwperf_finalize_, LWPERF_IMPL_BASE)
#define lwperf_add_invariant_impl LWPERF_CAT(lwperf_add_invariant_, LWPERF_IMPL_BASE)
#define lwperf_add_site_param_impl LWPERF_CAT(lwperf_add_site_param_, LWPERF_IMPL_BASE)
#define lwperf_log_impl LWPERF_CAT(lwperf_log_, LWPERF_IMPL_BASE)
#define lwperf_stop_impl LWPERF_CAT(lwperf_stop_, LWPERF_IMPL_BASE)
#define lwperf_init_papi_impl LWPERF_CAT(lwperf_init_papi_, LWPERF_IMPL_BASE)
#define lwperf_init_papi_with_names_impl LWPERF_CAT(lwperf_init_papi_with_names_, LWPERF_IMPL_BASE)

/* Backend implementations */

/* null backend */
typedef struct lwperf_null lwperf_null;
lwperf_null* lwperf_init_null(const char* machine, const char* application,
                          const char* dbname);
void lwperf_finalize_null(lwperf_null* perf);
void lwperf_add_invariant_null(lwperf_null* perf, const char* name, double value);
void lwperf_add_site_param_null(lwperf_null* perf, const char* site_name,
                                const char* param_name, double value);
void lwperf_log_null(lwperf_null* perf, const char* site_name);
void lwperf_stop_null(lwperf_null* perf, const char* site_name);
void lwperf_init_papi_null(lwperf_null* perf);
void lwperf_init_papi_with_names_null(lwperf_null* perf, const char** names, int num_names);

/* eiger backend */
typedef struct lwperf_eiger lwperf_eiger;
lwperf_eiger* lwperf_init_eiger(const char* machine, const char* application,
                           const char* dbname);
void lwperf_finalize_eiger(lwperf_eiger* perf);
void lwperf_add_invariant_eiger(lwperf_eiger* perf, const char* name, double value);
void lwperf_add_site_param_eiger(lwperf_eiger* perf, const char* site_name,
                                 const char* param_name, double value);
void lwperf_log_eiger(lwperf_eiger* perf, const char* site_name);
void lwperf_stop_eiger(lwperf_eiger* perf, const char* site_name);
void lwperf_init_papi_eiger(lwperf_eiger* perf);
void lwperf_init_papi_with_names_eiger(lwperf_eiger* perf, const char** names, int num_names);

/* csv backend */
typedef struct lwperf_csv lwperf_csv;
lwperf_csv* lwperf_init_csv(const char* machine, const char* application,
                           const char* dbname);
void lwperf_finalize_csv(lwperf_csv* perf);
void lwperf_add_invariant_csv(lwperf_csv* perf, const char* name, double value);
void lwperf_add_site_param_csv(lwperf_csv* perf, const char* site_name,
                                 const char* param_name, double value);
void lwperf_log_csv(lwperf_csv* perf, const char* site_name);
void lwperf_stop_csv(lwperf_csv* perf, const char* site_name);
void lwperf_init_papi_csv(lwperf_csv* perf);
void lwperf_init_papi_with_names_csv(lwperf_csv* perf, const char** names, int num_names);

/* sstmac backend */
typedef struct lwperf_sstmac lwperf_sstmac;
lwperf_sstmac* lwperf_init_sstmac(const char* machine, const char* application,
                                const char* dbname);
void lwperf_finalize_sstmac(lwperf_sstmac* perf);
void lwperf_add_invariant_sstmac(lwperf_sstmac* perf, const char* name, double value);
void lwperf_add_site_param_sstmac(lwperf_sstmac* perf, const char* site_name,
                                 const char* param_name, double value);
void lwperf_log_sstmac(lwperf_sstmac* perf, const char* site_name);
void lwperf_stop_sstmac(lwperf_sstmac* perf, const char* site_name);
void lwperf_init_papi_sstmac(lwperf_sstmac* perf);
void lwperf_init_papi_with_names_sstmac(lwperf_sstmac* perf, const char** names, int num_names);

/* Helper macros */
#define LWPERF_NARGS_SEQ(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, \
                         _13, _14, _15, _16, _17, _18, n, ...)              \
  n
#define LWPERF_NARGS(...)                                                    \
  LWPERF_NARGS_SEQ(__VA_ARGS__, 9, -1, 8, -1, 7, -1, 6, -1, 5, -1, 4, -1, 3, \
                   -1, 2, -1, 1, -1)

/* Convenience macro for adding multiple params in one go */
#define lwperf_add_site_params_impl(perf, name, ...)                        \
  LWPERF_CAT(lwperf_add_site_param_, LWPERF_NARGS(__VA_ARGS__))(perf, name, \
                                                                __VA_ARGS__)

/* lwperf_add_site_param_n declarations */
inline void lwperf_add_site_param_1(lwperf_t perf, const char* site_name,
                             const char* p1_name, double p1_val) {
  lwperf_add_site_param_impl(perf, site_name, p1_name, p1_val);
}
inline void lwperf_add_site_param_2(lwperf_t perf, const char* site_name,
                             const char* p1_name, double p1_val,
                             const char* p2_name, double p2_val) {
  lwperf_add_site_param_impl(perf, site_name, p1_name, p1_val);
  lwperf_add_site_param_impl(perf, site_name, p2_name, p2_val);
}
inline void lwperf_add_site_param_3(lwperf_t perf, const char* site_name,
                             const char* p1_name, double p1_val,
                             const char* p2_name, double p2_val,
                             const char* p3_name, double p3_val) {
  lwperf_add_site_param_impl(perf, site_name, p1_name, p1_val);
  lwperf_add_site_param_impl(perf, site_name, p2_name, p2_val);
  lwperf_add_site_param_impl(perf, site_name, p3_name, p3_val);
}
inline void lwperf_add_site_param_4(lwperf_t perf, const char* site_name,
                             const char* p1_name, double p1_val,
                             const char* p2_name, double p2_val,
                             const char* p3_name, double p3_val,
                             const char* p4_name, double p4_val) {
  lwperf_add_site_param_impl(perf, site_name, p1_name, p1_val);
  lwperf_add_site_param_impl(perf, site_name, p2_name, p2_val);
  lwperf_add_site_param_impl(perf, site_name, p3_name, p3_val);
  lwperf_add_site_param_impl(perf, site_name, p4_name, p4_val);
}
inline void lwperf_add_site_param_5(lwperf_t perf, const char* site_name,
                             const char* p1_name, double p1_val,
                             const char* p2_name, double p2_val,
                             const char* p3_name, double p3_val,
                             const char* p4_name, double p4_val,
                             const char* p5_name, double p5_val) {
  lwperf_add_site_param_impl(perf, site_name, p1_name, p1_val);
  lwperf_add_site_param_impl(perf, site_name, p2_name, p2_val);
  lwperf_add_site_param_impl(perf, site_name, p3_name, p3_val);
  lwperf_add_site_param_impl(perf, site_name, p4_name, p4_val);
  lwperf_add_site_param_impl(perf, site_name, p5_name, p5_val);
}
inline void lwperf_add_site_param_6(lwperf_t perf, const char* site_name,
                             const char* p1_name, double p1_val,
                             const char* p2_name, double p2_val,
                             const char* p3_name, double p3_val,
                             const char* p4_name, double p4_val,
                             const char* p5_name, double p5_val,
                             const char* p6_name, double p6_val) {
  lwperf_add_site_param_impl(perf, site_name, p1_name, p1_val);
  lwperf_add_site_param_impl(perf, site_name, p2_name, p2_val);
  lwperf_add_site_param_impl(perf, site_name, p3_name, p3_val);
  lwperf_add_site_param_impl(perf, site_name, p4_name, p4_val);
  lwperf_add_site_param_impl(perf, site_name, p5_name, p5_val);
  lwperf_add_site_param_impl(perf, site_name, p6_name, p6_val);
}
inline void lwperf_add_site_param_7(
    lwperf_t perf, const char* site_name, const char* p1_name, double p1_val,
    const char* p2_name, double p2_val, const char* p3_name, double p3_val,
    const char* p4_name, double p4_val, const char* p5_name, double p5_val,
    const char* p6_name, double p6_val, const char* p7_name, double p7_val) {
  lwperf_add_site_param_impl(perf, site_name, p1_name, p1_val);
  lwperf_add_site_param_impl(perf, site_name, p2_name, p2_val);
  lwperf_add_site_param_impl(perf, site_name, p3_name, p3_val);
  lwperf_add_site_param_impl(perf, site_name, p4_name, p4_val);
  lwperf_add_site_param_impl(perf, site_name, p5_name, p5_val);
  lwperf_add_site_param_impl(perf, site_name, p6_name, p6_val);
  lwperf_add_site_param_impl(perf, site_name, p7_name, p7_val);
}
inline void lwperf_add_site_param_8(
    lwperf_t perf, const char* site_name, const char* p1_name, double p1_val,
    const char* p2_name, double p2_val, const char* p3_name, double p3_val,
    const char* p4_name, double p4_val, const char* p5_name, double p5_val,
    const char* p6_name, double p6_val, const char* p7_name, double p7_val,
    const char* p8_name, double p8_val) {
  lwperf_add_site_param_impl(perf, site_name, p1_name, p1_val);
  lwperf_add_site_param_impl(perf, site_name, p2_name, p2_val);
  lwperf_add_site_param_impl(perf, site_name, p3_name, p3_val);
  lwperf_add_site_param_impl(perf, site_name, p4_name, p4_val);
  lwperf_add_site_param_impl(perf, site_name, p5_name, p5_val);
  lwperf_add_site_param_impl(perf, site_name, p6_name, p6_val);
  lwperf_add_site_param_impl(perf, site_name, p7_name, p7_val);
  lwperf_add_site_param_impl(perf, site_name, p8_name, p8_val);
}
inline void lwperf_add_site_param_9(
    lwperf_t perf, const char* site_name, const char* p1_name, double p1_val,
    const char* p2_name, double p2_val, const char* p3_name, double p3_val,
    const char* p4_name, double p4_val, const char* p5_name, double p5_val,
    const char* p6_name, double p6_val, const char* p7_name, double p7_val,
    const char* p8_name, double p8_val, const char* p9_name, double p9_val) {
  lwperf_add_site_param_impl(perf, site_name, p1_name, p1_val);
  lwperf_add_site_param_impl(perf, site_name, p2_name, p2_val);
  lwperf_add_site_param_impl(perf, site_name, p3_name, p3_val);
  lwperf_add_site_param_impl(perf, site_name, p4_name, p4_val);
  lwperf_add_site_param_impl(perf, site_name, p5_name, p5_val);
  lwperf_add_site_param_impl(perf, site_name, p6_name, p6_val);
  lwperf_add_site_param_impl(perf, site_name, p7_name, p7_val);
  lwperf_add_site_param_impl(perf, site_name, p8_name, p8_val);
  lwperf_add_site_param_impl(perf, site_name, p9_name, p9_val);
}
#ifdef __cplusplus
}
#endif
