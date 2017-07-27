AM_CXXFLAGS = -std=c++11 -I$(top_srcdir)/src

lib_LTLIBRARIES = libcnbicvsa.la

libcnbicvsa_includedir = $(includedir)/cnbicvsa
libcnbicvsa_include_HEADERS = InfiniteBar.hpp

libcnbicvsa_la_SOURCES = InfiniteBar.hpp \
						 InfiniteBar.cpp 

libcnbicvsa_la_LIBADD = -ldrawtk -lcnbicore -lcnbidraw
libcnbicvsa_la_LDFLAGS = $(AM_CFLAGS) \
						-version-info $(CURRENT):$(REVISION):$(AGE)

noinst_PROGRAMS = sandbox

sandbox_SOURCES = sandbox.cpp
sandbox_LDADD = -lcnbicore -ldrawtk -lcnbidraw $(top_builddir)/src/libcnbicvsa.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cnbicvsa.pc

