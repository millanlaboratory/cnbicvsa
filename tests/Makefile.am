AM_CXXFLAGS = -std=c++11 -I$(top_srcdir)/src

check_PROGRAMS = test_colorfeedback test_targetcontrol

test_colorfeedback_SOURCES = test_colorfeedback.cpp
test_colorfeedback_LDADD = -lcnbicore -ldrawtk -lcnbidraw $(top_builddir)/src/libcnbicvsa.la

test_targetcontrol_SOURCES = test_targetcontrol.cpp
test_targetcontrol_LDADD = -lcnbicore -ldrawtk -lcnbidraw $(top_builddir)/src/libcnbicvsa.la

#TESTS = test_engine test_shapes test_images test_strings test_events


