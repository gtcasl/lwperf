lib_LTLIBRARIES = liblwperf.la
pkginclude_HEADERS = lwperf.h lwperf_impl.h
nodist_include_HEADERS = lwperf_config.h
liblwperf_la_SOURCES = csvbackend.cpp lwperf_impl.cpp
if ENABLE_EIGER
liblwperf_la_SOURCES += eigerbackend.cpp
endif
if ENABLE_SSTMAC
liblwperf_la_SOURCES += sstmacbackend.cpp
endif
