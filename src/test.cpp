#include "newperf.h"

void compute1(int n) {(void)n;}

void compute2(int m, int n) {(void)m; (void)n;}

void comm() {}

int main() {

  lwperf::init("m", "a", "d", "p", "s");
  

  int n = 1000;
  lwperf::start("compute1", n);
  compute1(n);
  lwperf::stop("compute1");

  comm();

  int m = 100;
  {
    lwperf::log logger("compute2", m, n);
    compute2(m, n);
  }

  lwperf::finalize();

}

