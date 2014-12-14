#include "eigerbackend.h"

#include "lwperf_impl.h"

template<typename Backend>
class lwperf {
  private:
    Backend backend;
  public:
    template<typename... Args>
    lwperf(Args&&... args) : backend{std::forward<Args>(args)...} {}
    template<typename... Args>
    void add_invariant(Args&&... args) {
      backend.add_invariant(std::forward<Args>(args)...);
    }
    template<typename... Args>
    void add_cite_param(Args&&... args) {
      backend.add_cite_param(std::forward<Args>(args)...);
    }
    template<typename... Args>
    void log(Args&&... args) {
      backend.log(std::forward<Args>(args)...);
    }
    template<typename... Args>
    void stop(Args&&... args) {
      backend.stop(std::forward<Args>(args)...);
    }
};

class lwperf_eiger : public lwperf<EigerBackend> {
  using lwperf::lwperf;
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

}

