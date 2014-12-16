AC_INIT([lwperf], [2.0], [eanger@gatech.edu])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_MACRO_DIR([acinclude])
AM_PROG_AR

AC_PROG_CXX

LT_INIT

# c++11
AX_CXX_COMPILE_STDCXX_11

# eiger
AC_ARG_WITH([eiger-include-path],
			[AS_HELP_STRING([--with-eiger-include-path],
							[location of the eiger headers, defaults to /usr/include])],
			[EIGER_CFLAGS="-I$withval"],
			[EIGER_CFLAGS='-I/usr/include'])
AC_SUBST([EIGER_CFLAGS])

AC_ARG_WITH([eiger-lib-path],
			[AS_HELP_STRING([--with-eiger-lib-path],
							[location of the eiger library])],
			[EIGER_LIBS="-L$withval -leiger"],
			[EIGER_LIBS='-leiger'])
AC_SUBST([EIGER_LIBS])

AC_CONFIG_HEADERS([config/config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
