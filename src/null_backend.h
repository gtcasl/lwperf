#pragma once
#include <iostream>

#include "backend.h"

using namespace std;

struct lwperf_null {
  lwperf_null() {
    cout << "constructor" << endl;
  }

  ~lwperf_null() {
    cout << "destructor" << endl;
  }

  void stop(const char* cite_name) {
    (void)cite_name;
    cout << "stop" << endl;
  }

  void add_invariant(const char* name, double value) {
    cout << "add invariant" << endl;
  }

  void add_cite_param(const char* cite_name, const char* param_name,
                      double value) {
    cout << "add cite param" << endl;
  }

  void log(const char* cite_name) {
    cout << "log" << endl;
  }
};
