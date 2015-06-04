#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#define LWPERF_USE_CSV
#include "lwperf.h"

using namespace std;

int main(int argc, char* argv[], char* envp[]){

  auto usage = 
    "Usage:\n"
    " lwperf-wrapper [options] <executable>\n"
    "\n"
    "Options:\n"
    " -h                      Show this help\n"
    " -p <counter>            Name of PAPI counter to initialize lwperf with. Can have multiple\n"
    " -i <invariant>=<value>  Pass invariant name--value pairs to lwperf to use for this run.\n"
    "\n";

  vector<string> papi_counter_strings;
  map<string, double> invariants;

  int param;
  while((param = getopt(argc, argv, "+hp:i:")) != -1){
    switch(param){
      case 'p':{
        papi_counter_strings.emplace_back(optarg);
      } break;
      case 'i':{
        string val = optarg;
        auto sep_idx = val.find('=');
        string ivar_name = val.substr(0, sep_idx);
        double ivar_val = stod(val.substr(sep_idx + 1));
        invariants[ivar_name] = ivar_val;
      } break;
      case 'h':
      case '?':
        cout << usage;
        return 0;
      default:
        cerr << "Error: bad getopt parse of parameter.\n";
        return -1;
    }
  }

  // start counting energy
  auto perf = lwperf_init("", "", "lwperf-wrapped.db");
  vector<const char*> papi_counters;
  transform(begin(papi_counter_strings), end(papi_counter_strings),
            back_inserter(papi_counters),
            [&](const string& s) { return s.c_str(); });
  if(!papi_counters.empty()){
    lwperf_init_papi_with_names(perf, &papi_counters[0], papi_counters.size());
  }
  for(const auto& ivar : invariants){
    lwperf_add_invariant(perf, ivar.first.c_str(), ivar.second);
  }
  lwperf_log(perf, "wrapped");

  auto child_pid = fork();
  if(child_pid == -1){
    cerr << "Error forking new process\n";
    return -1;
  }

  if (child_pid == 0){ // child
    execve(argv[optind], &argv[optind], envp);
  } else { // parent
    int status;
    // wait on child to terminate
    while(true){
      wait(&status);
      if(WIFEXITED(status)){
        break;
      }
    }
    // stop counting energy and report
    lwperf_stop(perf, "wrapped");
    lwperf_finalize(perf);
  }

  return 0;
}

