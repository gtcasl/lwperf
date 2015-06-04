lib_LTLIBRARIES = liblwperf.la
pkginclude_HEADERS = lwperf.h lwperf_impl.h lwperf_config.h
liblwperf_la_SOURCES = csvbackend.cpp lwperf_impl.cpp
if ENABLE_EIGER
liblwperf_la_SOURCES += eigerbackend.cpp
endif
if ENABLE_SSTMAC
liblwperf_la_SOURCES += sstmacbackend.cpp
endif

#liblwperf_la_CXXFLAGS = -fvisibility=hidden
liblwperf_la_LDFLAGS = $(SSTMAC_LIBS) -export-symbols-regex '^lwperf_'

bin_PROGRAMS = lwperf-wrapper
lwperf_wrapper_SOURCES = lwperf_wrapper.cpp
#lwperf-wrapper_CPPFLAGS = -I$(srcdir)
lwperf_wrapper_LDADD = liblwperf.la
