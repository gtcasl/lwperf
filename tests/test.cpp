#include <unistd.h>
#include <iostream>

#include <lwperf.h>

using namespace std;

int main() {
  
  cout << "Initializing lwperf\n";
  lwperf_t perf = lwperf_init("harrenhal", "test", "database", "", "");

  cout << "Adding first invariant\n";
  lwperf_add_invariant(perf, "invariant1", 123);

  cout << "Begin looping for 5 iterations\n";
  for(int i = 0; i < 5; ++i){
    cout << "Adding second invariant\n";
    lwperf_add_invariant(perf, "invariant2", 456);

    cout << "Adding first cite param\n";
    lwperf_add_cite_param(perf, "cite1", "param1", 111);
    cout << "Adding second and third cite param\n";
    lwperf_add_cite_params(perf, "cite1", "param2", i, "param3", i*i);

    cout << "Begin log\n";
    lwperf_log(perf, "cite1");
    cout << "...sleeping\n";
    usleep(500000);
    cout << "End log\n";
    lwperf_stop(perf, "cite1");
  }

  cout << "Finalizing\n";
  lwperf_finalize(perf);
}

