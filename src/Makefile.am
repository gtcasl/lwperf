AM_CXXFLAGS = -std=c++11

lib_LTLIBRARIES = liblwperf.la
pkginclude_HEADERS = lwperf.h lwperf_impl.h
liblwperf_la_SOURCES = eigerbackend.cpp lwperf_impl.cpp
liblwperf_la_CPPFLAGS = $(EIGER_CFLAGS)
liblwperf_la_LIBADD = $(EIGER_LIBS)

ACLOCAL_AMFLAGS = -I m4
