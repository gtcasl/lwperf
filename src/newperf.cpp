#include "newperf.h"

#include <memory>

namespace lwperf{

namespace {
Logger* logger;
} // end unnamed namespace

void init(std::string machine, std::string app, std::string dbname,
          std::string prefix, std::string suffix) {
  logger = new Logger(machine, app, dbname, prefix, suffix);
}
void finalize() {
  delete logger;
}

} // end namespace lwperf

