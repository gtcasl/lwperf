#pragma once

struct lwperf_ {
  virtual ~lwperf_() = 0;
  virtual void stop(const char* cite_name) = 0;
  virtual void add_invariant(const char* name, double value) = 0;
  virtual void add_cite_param(const char* cite_name, const char* param_name,
                              double value) = 0;
  virtual void log(const char* cite_name) = 0;
};
