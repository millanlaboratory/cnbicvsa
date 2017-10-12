AM_CXXFLAGS = -std=c++11 -I$(top_srcdir)/src -O0 -fbuiltin -g

lib_LTLIBRARIES = libcnbicvsa.la

libcnbicvsa_includedir = $(includedir)/cnbicvsa
libcnbicvsa_include_HEADERS = InfiniteBar.hpp \
						 	  ColorFeedback.hpp \
						 	  Target.hpp \
							  Copilot.hpp

libcnbicvsa_la_SOURCES = InfiniteBar.hpp \
						 InfiniteBar.cpp \
						 ColorFeedback.hpp \
						 ColorFeedback.cpp \
						 Target.hpp \
						 Target.cpp \
						 Copilot.hpp \
						 Copilot.cpp

libcnbicvsa_la_LIBADD = -ldrawtk -lcnbicore -lcnbidraw
libcnbicvsa_la_LDFLAGS = $(AM_CFLAGS) \
						-version-info $(CURRENT):$(REVISION):$(AGE)


bin_PROGRAMS = cvsa_offline cvsa_neglect_offline
cvsa_offline_SOURCES = cvsa_offline.cpp cvsa_utilities.hpp
cvsa_offline_LDADD = -lcnbicore 	\
					 -lcnbiloop 	\
					 -ltobicore 	\
					 -ltobiid 		\
					 -llpttrigger 	\
					 -lcnbiconfig 	\
					 -ldrawtk 		\
					 -lcnbidraw 	\
					 $(top_builddir)/src/libcnbicvsa.la

cvsa_neglect_offline_SOURCES = cvsa_neglect_offline.cpp cvsa_utilities.hpp
cvsa_neglect_offline_LDADD = -lcnbicore 	\
					 		 -lcnbiloop 	\
					 		 -ltobicore 	\
					 		 -ltobiid 		\
					 		 -llpttrigger 	\
					 		 -lcnbiconfig 	\
					 		 -ldrawtk 		\
					 		 -lcnbidraw 	\
					 		 $(top_builddir)/src/libcnbicvsa.la

noinst_PROGRAMS = sandbox infinitebar
sandbox_SOURCES = sandbox.cpp
sandbox_LDADD = -lcnbicore -ldrawtk -lcnbidraw $(top_builddir)/src/libcnbicvsa.la

infinitebar_SOURCES = infinitebar.cpp
infinitebar_LDADD = -lcnbicore -ldrawtk -lcnbidraw $(top_builddir)/src/libcnbicvsa.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cnbicvsa.pc

