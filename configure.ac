m4_define([lib_current],1)
m4_define([lib_revision],0)
m4_define([lib_age],0)

AC_INIT(cnbicvsa,[lib_current.lib_revision.lib_age],
		luca.tonin@epfl.ch)
AC_CONFIG_SRCDIR([src/InfiniteBar.hpp])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config/config.h])

AM_INIT_AUTOMAKE([no-dist-gzip dist-bzip2 color-tests subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CANONICAL_HOST
AC_CANONICAL_BUILD

AC_PROG_LIBTOOL
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11
CFLAGS+=" -std=c11"
LT_INIT([win32-dll disable-static])
AC_SUBST(LIBTOOL_DEPS)

save_LIBS="$LIBS"

AC_SEARCH_LIBS_CLASS([CcFile], [cnbicore], [#include <cnbicore/CcFile.hpp>], ,
			AC_MSG_ERROR([The cnbicore library cannot be found.]))
PKG_CHECK_MODULES(CNBIDRAW, [cnbidraw], [#include <cnbidraw/Shape.hpp>], ,
			AC_MSG_ERROR([The cnbidraw library cannot be found.]))
LIBS="$save_LIBS"

AC_CHECK_PROG([DOXYGEN], [doxygen], [doxygen] [no])
if test "$DOXYGEN" = no ; then 
	AC_MSG_ERROR(Cannot find doxygen.) 
fi

AC_HEADER_STDBOOL

AC_SUBST([RELEASE],[lib_release])
AC_SUBST([CURRENT],[lib_current])
AC_SUBST([REVISION],[lib_revision])
AC_SUBST([AGE],[lib_age])
AC_SUBST(AM_LDFLAGS)

AC_CONFIG_FILES([
Makefile 
src/Makefile
src/cnbicvsa.pc])
AC_OUTPUT
