#include "lwperf_config.h"
#include "logger.h"
#ifdef LWPERF_HAVE_EIGER
#include "eigerbackend.h"
#endif
#include "csvbackend.h"
#ifdef LWPERF_HAVE_SSTMAC
#include "sstmacbackend.h"
#endif

#include "lwperf_impl.h"

#ifdef LWPERF_HAVE_EIGER
struct lwperf_eiger : public lwperf::Logger<lwperf::EigerBackend> {
  using Logger::Logger;
};
#endif

struct lwperf_csv : public lwperf::Logger<lwperf::CSVBackend> {
  using Logger::Logger;
};

#ifdef LWPERF_HAVE_SSTMAC
struct lwperf_sstmac : public lwperf::Logger<lwperf::SSTMACBackend>{
  using Logger::Logger;
};
#endif

extern "C" {

/* null API */
lwperf_null* lwperf_init_null(const char* machine, const char* application,
                          const char* dbname) {
  (void)machine;
  (void)application;
  (void)dbname;
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

void lwperf_add_site_param_null(lwperf_null* perf, const char* site_name,
                                const char* param_name, double value) {
  (void)perf;
  (void)site_name;
  (void)param_name;
  (void)value;
}

void lwperf_log_null(lwperf_null* perf, const char* site_name) {
  (void)perf;
  (void)site_name;
}

void lwperf_stop_null(lwperf_null* perf, const char* site_name) {
  (void)perf;
  (void)site_name;
}

void lwperf_init_papi_null(lwperf_null* perf) {
  (void)perf;
}

#ifdef LWPERF_HAVE_EIGER
/* eiger backend */
lwperf_eiger* lwperf_init_eiger(const char* machine, const char* application,
                           const char* dbname) {
  auto perf = new lwperf_eiger(machine, application, dbname);
  perf->enable_measurement();
  return perf;
}

void lwperf_finalize_eiger(lwperf_eiger* perf) {
  delete perf;
}

void lwperf_add_invariant_eiger(lwperf_eiger* perf, const char* name, double value) {
  perf->add_invariant(name, value);
}

void lwperf_add_site_param_eiger(lwperf_eiger* perf, const char* site_name,
                                 const char* param_name, double value) {
  perf->add_site_param(site_name, param_name, value);
}

void lwperf_log_eiger(lwperf_eiger* perf, const char* site_name) {
  perf->log(site_name);
}

void lwperf_stop_eiger(lwperf_eiger* perf, const char* site_name) {
  perf->stop(site_name);
}

void lwperf_init_papi_eiger(lwperf_eiger* perf) {
  perf->init_papi();
}
#endif

/* csv backend */
lwperf_csv* lwperf_init_csv(const char* machine, const char* application,
                           const char* dbname) {
  auto perf =  new lwperf_csv(machine, application, dbname);
  perf->enable_measurement();
  return perf;
}

void lwperf_finalize_csv(lwperf_csv* perf) {
  delete perf;
}

void lwperf_add_invariant_csv(lwperf_csv* perf, const char* name, double value) {
  perf->add_invariant(name, value);
}

void lwperf_add_site_param_csv(lwperf_csv* perf, const char* site_name,
                                 const char* param_name, double value) {
  perf->add_site_param(site_name, param_name, value);
}

void lwperf_log_csv(lwperf_csv* perf, const char* site_name) {
  perf->log(site_name);
}

void lwperf_stop_csv(lwperf_csv* perf, const char* site_name) {
  perf->stop(site_name);
}

void lwperf_init_papi_csv(lwperf_csv* perf) {
  perf->init_papi();
}

#ifdef LWPERF_HAVE_SSTMAC
/* sstmac backend */
lwperf_sstmac* lwperf_init_sstmac(const char* machine, const char* application,
                                const char* dbname) {
  return new lwperf_sstmac(machine, application, dbname);
}

void lwperf_finalize_sstmac(lwperf_sstmac* perf) {
  delete perf;
}

void lwperf_add_invariant_sstmac(lwperf_sstmac* perf, const char* name, double value) {
  perf->add_invariant(name, value);
}

void lwperf_add_site_param_sstmac(lwperf_sstmac* perf, const char* site_name,
                                 const char* param_name, double value) {
  perf->add_site_param(site_name, param_name, value);
}

void lwperf_log_sstmac(lwperf_sstmac* perf, const char* site_name) {
  perf->log(site_name);
}

void lwperf_stop_sstmac(lwperf_sstmac* perf, const char* site_name) {
  perf->stop(site_name);
}

void lwperf_init_papi_sstmac(lwperf_sstmac* perf) {
  perf->init_papi();
}
#endif

}

