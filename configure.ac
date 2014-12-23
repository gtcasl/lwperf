AC_INIT([lwperf], [2.0], [eanger@gatech.edu])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_MACRO_DIR([acinclude])
AM_PROG_AR

AC_PROG_CXX

LT_INIT

# c++11
AX_CXX_COMPILE_STDCXX_11

AC_ARG_ENABLE([papi],
              AS_HELP_STRING([--enable-papi], [Enable PAPI counter measurement]))

AS_IF([test "x$enable_papi" = "xyes"], [
AC_SEARCH_LIBS([PAPI_library_init], [papi], [], [AC_MSG_ERROR([papi is not installed])])
HAVE_PAPI_STRING="#define LWPERF_HAVE_PAPI"
AC_CHECK_HEADERS([papi.h])        # HAVE_PAPI_H
], [
HAVE_PAPI_STRING="#undef LWPERF_HAVE_PAPI"
])
AC_SUBST(HAVE_PAPI, $HAVE_PAPI_STRING)
AM_CONDITIONAL([ENABLE_PAPI], [test "x$enable_papi" = "xyes"])

AC_ARG_ENABLE([eiger],
              AS_HELP_STRING([--enable-eiger], [Enable Eiger backend]))

AS_IF([test "x$enable_eiger" = "xyes"], [
AC_LANG_PUSH([C++])
SAVED_LDFLAGS=$LDFLAGS
AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([#include <eiger/eiger.h>],
                     [eiger::error_t dummy])],
    [TEST_LIBS="$TEST_LIBS -leiger"] [HAVE_EIGER=1],
    [AC_MSG_ERROR([Eiger is not installed.])])
LDFLAGS=$SAVED_LDFLAGS
AC_CHECK_HEADERS([eiger/eiger.h]) # HAVE_EIGER_EIGER_H
HAVE_EIGER_STRING="#define LWPERF_HAVE_EIGER"
AC_LANG_POP([C++])
], [
HAVE_EIGER_STRING="#undef LWPERF_HAVE_EIGER"
])
AC_SUBST(HAVE_EIGER, $HAVE_EIGER_STRING)
AM_CONDITIONAL([ENABLE_EIGER], [test "x$enable_eiger" = "xyes"])

AC_CONFIG_FILES([Makefile src/Makefile src/lwperf_config.h])
AC_CONFIG_HEADERS([config/config.h])
AC_OUTPUT
