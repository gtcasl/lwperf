#include "logger.h"
#include <cassert>

namespace lwperf {

Logger::Logger(lwperf_backend_t backend, const char* machine,
               const char* application, const char* dbname, const char* prefix,
               const char* suffix) {
  switch(backend) {
    case NULL_BACKEND:
      break;
    case EIGER_BACKEND:
      break;
    case FAKE_BACKEND:
      break;
    default:
      assert(0 && "Error: this backend type has not been implemented.");
  }
}

} // end namespace lwperf
