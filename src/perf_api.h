#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_USE_EIGER)
typedef struct lwperf_eiger* lwperf_t;
#elif defined(_USE_FAKEEIGER)
typedef struct lwperf_fake* lwperf_t;
#elif defined(_USE_EIGER_MODEL)
typedef struct lwperf_model* lwperf_t;
#else
typedef struct lwperf_null* lwperf_t;
#endif

lwperf_t lwperf_init(const char* machine, const char* application,
                     const char* dbname, const char* prefix,
                     const char* suffix);
void lwperf_finalize(lwperf_t perf);

void lwperf_stop(lwperf_t perf, const char* cite_name);

void lwperf_add_invariant(lwperf_t perf, const char* name, double value);
void lwperf_add_cite_param(lwperf_t perf, const char* cite_name,
                           const char* param_name, double value);

void lwperf_log(lwperf_t perf, const char* cite_name);

/* Helper macros */
#define LWPERF_NARGS_SEQ(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, \
                         _13, _14, _15, _16, _17, _18, n, ...)              \
  n
#define LWPERF_NARGS(...)                                                    \
  LWPERF_NARGS_SEQ(__VA_ARGS__, 9, -1, 8, -1, 7, -1, 6, -1, 5, -1, 4, -1, 3, \
                   -1, 2, -1, 1, -1)

#define LWPERF_PRIMITIVE_CAT(x, y) x ## y
#define LWPERF_CAT(x,y) LWPERF_PRIMITIVE_CAT(x,y)

/* Convenience macro for adding multiple params in one go */
#define lwperf_add_cite_params(perf, name, ...)                             \
  LWPERF_CAT(lwperf_add_cite_param_, LWPERF_NARGS(__VA_ARGS__))(perf, name, \
                                                                __VA_ARGS__)

/* lwperf_add_cite_param_n declarations */
void lwperf_add_cite_param_1(lwperf_t perf, const char* cite_name,
                             const char* p1_name, double p1_val) {
  lwperf_add_cite_param(perf, cite_name, p1_name, p1_val);
}
void lwperf_add_cite_param_2(lwperf_t perf, const char* cite_name,
                             const char* p1_name, double p1_val,
                             const char* p2_name, double p2_val) {
  lwperf_add_cite_param(perf, cite_name, p1_name, p1_val);
  lwperf_add_cite_param(perf, cite_name, p2_name, p2_val);
}
void lwperf_add_cite_param_3(lwperf_t perf, const char* cite_name,
                             const char* p1_name, double p1_val,
                             const char* p2_name, double p2_val,
                             const char* p3_name, double p3_val) {
  lwperf_add_cite_param(perf, cite_name, p1_name, p1_val);
  lwperf_add_cite_param(perf, cite_name, p2_name, p2_val);
  lwperf_add_cite_param(perf, cite_name, p3_name, p3_val);
}
void lwperf_add_cite_param_4(lwperf_t perf, const char* cite_name,
                             const char* p1_name, double p1_val,
                             const char* p2_name, double p2_val,
                             const char* p3_name, double p3_val,
                             const char* p4_name, double p4_val) {
  lwperf_add_cite_param(perf, cite_name, p1_name, p1_val);
  lwperf_add_cite_param(perf, cite_name, p2_name, p2_val);
  lwperf_add_cite_param(perf, cite_name, p3_name, p3_val);
  lwperf_add_cite_param(perf, cite_name, p4_name, p4_val);
}
void lwperf_add_cite_param_5(lwperf_t perf, const char* cite_name,
                             const char* p1_name, double p1_val,
                             const char* p2_name, double p2_val,
                             const char* p3_name, double p3_val,
                             const char* p4_name, double p4_val,
                             const char* p5_name, double p5_val) {
  lwperf_add_cite_param(perf, cite_name, p1_name, p1_val);
  lwperf_add_cite_param(perf, cite_name, p2_name, p2_val);
  lwperf_add_cite_param(perf, cite_name, p3_name, p3_val);
  lwperf_add_cite_param(perf, cite_name, p4_name, p4_val);
  lwperf_add_cite_param(perf, cite_name, p5_name, p5_val);
}
void lwperf_add_cite_param_6(lwperf_t perf, const char* cite_name,
                             const char* p1_name, double p1_val,
                             const char* p2_name, double p2_val,
                             const char* p3_name, double p3_val,
                             const char* p4_name, double p4_val,
                             const char* p5_name, double p5_val,
                             const char* p6_name, double p6_val) {
  lwperf_add_cite_param(perf, cite_name, p1_name, p1_val);
  lwperf_add_cite_param(perf, cite_name, p2_name, p2_val);
  lwperf_add_cite_param(perf, cite_name, p3_name, p3_val);
  lwperf_add_cite_param(perf, cite_name, p4_name, p4_val);
  lwperf_add_cite_param(perf, cite_name, p5_name, p5_val);
  lwperf_add_cite_param(perf, cite_name, p6_name, p6_val);
}
void lwperf_add_cite_param_7(
    lwperf_t perf, const char* cite_name, const char* p1_name, double p1_val,
    const char* p2_name, double p2_val, const char* p3_name, double p3_val,
    const char* p4_name, double p4_val, const char* p5_name, double p5_val,
    const char* p6_name, double p6_val, const char* p7_name, double p7_val) {
  lwperf_add_cite_param(perf, cite_name, p1_name, p1_val);
  lwperf_add_cite_param(perf, cite_name, p2_name, p2_val);
  lwperf_add_cite_param(perf, cite_name, p3_name, p3_val);
  lwperf_add_cite_param(perf, cite_name, p4_name, p4_val);
  lwperf_add_cite_param(perf, cite_name, p5_name, p5_val);
  lwperf_add_cite_param(perf, cite_name, p6_name, p6_val);
  lwperf_add_cite_param(perf, cite_name, p7_name, p7_val);
}
void lwperf_add_cite_param_8(
    lwperf_t perf, const char* cite_name, const char* p1_name, double p1_val,
    const char* p2_name, double p2_val, const char* p3_name, double p3_val,
    const char* p4_name, double p4_val, const char* p5_name, double p5_val,
    const char* p6_name, double p6_val, const char* p7_name, double p7_val,
    const char* p8_name, double p8_val) {
  lwperf_add_cite_param(perf, cite_name, p1_name, p1_val);
  lwperf_add_cite_param(perf, cite_name, p2_name, p2_val);
  lwperf_add_cite_param(perf, cite_name, p3_name, p3_val);
  lwperf_add_cite_param(perf, cite_name, p4_name, p4_val);
  lwperf_add_cite_param(perf, cite_name, p5_name, p5_val);
  lwperf_add_cite_param(perf, cite_name, p6_name, p6_val);
  lwperf_add_cite_param(perf, cite_name, p7_name, p7_val);
  lwperf_add_cite_param(perf, cite_name, p8_name, p8_val);
}
void lwperf_add_cite_param_9(
    lwperf_t perf, const char* cite_name, const char* p1_name, double p1_val,
    const char* p2_name, double p2_val, const char* p3_name, double p3_val,
    const char* p4_name, double p4_val, const char* p5_name, double p5_val,
    const char* p6_name, double p6_val, const char* p7_name, double p7_val,
    const char* p8_name, double p8_val, const char* p9_name, double p9_val) {
  lwperf_add_cite_param(perf, cite_name, p1_name, p1_val);
  lwperf_add_cite_param(perf, cite_name, p2_name, p2_val);
  lwperf_add_cite_param(perf, cite_name, p3_name, p3_val);
  lwperf_add_cite_param(perf, cite_name, p4_name, p4_val);
  lwperf_add_cite_param(perf, cite_name, p5_name, p5_val);
  lwperf_add_cite_param(perf, cite_name, p6_name, p6_val);
  lwperf_add_cite_param(perf, cite_name, p7_name, p7_val);
  lwperf_add_cite_param(perf, cite_name, p8_name, p8_val);
  lwperf_add_cite_param(perf, cite_name, p9_name, p9_val);
}
#ifdef __cplusplus
} // extern "C"
#endif
