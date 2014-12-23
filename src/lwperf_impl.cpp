#include "lwperf_config.h"
#include "logger.h"
#ifdef LWPERF_HAVE_EIGER
#include "eigerbackend.h"
#endif
#include "csvbackend.h"
#include "modelcaller.h"

#include "lwperf_impl.h"

#ifdef LWPERF_HAVE_EIGER
struct lwperf_eiger : public lwperf::Logger<lwperf::EigerBackend> {
  using Logger::Logger;
};
#endif

struct lwperf_csv : public lwperf::Logger<lwperf::CSVBackend> {
  using Logger::Logger;
};

struct lwperf_model : public lwperf::ModelCaller{
  using ModelCaller::ModelCaller;
};

extern "C" {

/* null API */
lwperf_null* lwperf_init_null(const char* machine, const char* application,
                          const char* dbname, const char* prefix,
                          const char* suffix) {
  (void)machine;
  (void)application;
  (void)dbname;
  (void)prefix;
  (void)suffix;
  return nullptr;
}

void lwperf_finalize_null(lwperf_null* perf) {
  (void)perf;
}

void lwperf_add_invariant_null(lwperf_null* perf, const char* name, double value) {
  (void)perf;
  (void)name;
  (void)value;
}

void lwperf_add_cite_param_null(lwperf_null* perf, const char* cite_name,
                                const char* param_name, double value) {
  (void)perf;
  (void)cite_name;
  (void)param_name;
  (void)value;
}

void lwperf_log_null(lwperf_null* perf, const char* cite_name) {
  (void)perf;
  (void)cite_name;
}

void lwperf_stop_null(lwperf_null* perf, const char* cite_name) {
  (void)perf;
  (void)cite_name;
}

#ifdef LWPERF_HAVE_EIGER
/* eiger backend */
lwperf_eiger* lwperf_init_eiger(const char* machine, const char* application,
                           const char* dbname, const char* prefix,
                           const char* suffix) {
  return new lwperf_eiger(machine, application, dbname, prefix, suffix);
}

void lwperf_finalize_eiger(lwperf_eiger* perf) {
  delete perf;
}

void lwperf_add_invariant_eiger(lwperf_eiger* perf, const char* name, double value) {
  perf->add_invariant(name, value);
}

void lwperf_add_cite_param_eiger(lwperf_eiger* perf, const char* cite_name,
                                 const char* param_name, double value) {
  perf->add_cite_param(cite_name, param_name, value);
}

void lwperf_log_eiger(lwperf_eiger* perf, const char* cite_name) {
  perf->log(cite_name);
}

void lwperf_stop_eiger(lwperf_eiger* perf, const char* cite_name) {
  perf->stop(cite_name);
}
#endif

/* csv backend */
lwperf_csv* lwperf_init_csv(const char* machine, const char* application,
                           const char* dbname, const char* prefix,
                           const char* suffix) {
  return new lwperf_csv(machine, application, dbname, prefix, suffix);
}

void lwperf_finalize_csv(lwperf_csv* perf) {
  delete perf;
}

void lwperf_add_invariant_csv(lwperf_csv* perf, const char* name, double value) {
  perf->add_invariant(name, value);
}

void lwperf_add_cite_param_csv(lwperf_csv* perf, const char* cite_name,
                                 const char* param_name, double value) {
  perf->add_cite_param(cite_name, param_name, value);
}

void lwperf_log_csv(lwperf_csv* perf, const char* cite_name) {
  perf->log(cite_name);
}

void lwperf_stop_csv(lwperf_csv* perf, const char* cite_name) {
  perf->stop(cite_name);
}

/* model backend */
lwperf_model* lwperf_init_model(const char* machine, const char* application,
                                const char* dbname, const char* prefix,
                                const char* suffix) {
  return new lwperf_model(machine, application, dbname, prefix, suffix);
}

void lwperf_finalize_model(lwperf_model* perf) {
  delete perf;
}

void lwperf_add_invariant_model(lwperf_model* perf, const char* name, double value) {
  perf->add_invariant(name, value);
}

void lwperf_add_cite_param_model(lwperf_model* perf, const char* cite_name,
                                 const char* param_name, double value) {
  perf->add_cite_param(cite_name, param_name, value);
}

void lwperf_log_model(lwperf_model* perf, const char* cite_name) {
  perf->log(cite_name);
}

void lwperf_stop_model(lwperf_model* perf, const char* cite_name) {
  perf->stop(cite_name);
}

}

