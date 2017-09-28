AM_CXXFLAGS = -std=c++11 -I$(top_srcdir)/src

lib_LTLIBRARIES = libcnbicvsa.la

libcnbicvsa_includedir = $(includedir)/cnbicvsa
libcnbicvsa_include_HEADERS = InfiniteBar.hpp \
						 	  ColorFeedback.hpp \
						 	  TargetControl.hpp

libcnbicvsa_la_SOURCES = InfiniteBar.hpp \
						 InfiniteBar.cpp \
						 ColorFeedback.hpp \
						 ColorFeedback.cpp \
						 TargetControl.hpp \
						 TargetControl.cpp

libcnbicvsa_la_LIBADD = -ldrawtk -lcnbicore -lcnbidraw
libcnbicvsa_la_LDFLAGS = $(AM_CFLAGS) \
						-version-info $(CURRENT):$(REVISION):$(AGE)


bin_PROGRAMS = cvsa_feedback_offline
cvsa_feedback_offline_SOURCES = cvsa_feedback_offline.cpp
cvsa_feedback_offline_LDADD = -lcnbicore -ldrawtk -lcnbidraw $(top_builddir)/src/libcnbicvsa.la

noinst_PROGRAMS = sandbox infinitebar
sandbox_SOURCES = sandbox.cpp
sandbox_LDADD = -lcnbicore -ldrawtk -lcnbidraw $(top_builddir)/src/libcnbicvsa.la

infinitebar_SOURCES = infinitebar.cpp
infinitebar_LDADD = -lcnbicore -ldrawtk -lcnbidraw $(top_builddir)/src/libcnbicvsa.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cnbicvsa.pc

