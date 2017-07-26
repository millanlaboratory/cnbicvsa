AM_CXXFLAGS = -std=c++11 -I/usr/include/eigen3

lib_LTLIBRARIES = libcnbidraw.la

libcnbidraw_includedir = $(includedir)/cnbidraw
libcnbidraw_include_HEADERS = Flags.hpp \
							  Engine.hpp \
							  Window.hpp \
							  Events.hpp \
							  EventKeyboard.hpp \
							  EventMouse.hpp \
							  Shape.hpp \
							  ShapeMap.hpp \
						 	  Arrow.hpp \
						 	  Circle.hpp \
						 	  Cross.hpp \
						 	  Rectangle.hpp \
						 	  Ring.hpp \
							  Image.hpp \
							  String.hpp \
							  Font.hpp

libcnbidraw_la_SOURCES = Flags.hpp \
						 Engine.hpp \
						 Engine.cpp \
						 Window.hpp \
						 Window.cpp \
						 Events.hpp \
						 Events.cpp \
						 EventKeyboard.hpp \
						 EventKeyboard.cpp \
						 EventMouse.hpp \
						 EventMouse.cpp \
						 Shape.hpp \
						 Shape.cpp \
						 ShapeMap.hpp \
						 ShapeMap.cpp \
						 Arrow.hpp \
						 Arrow.cpp \
						 Circle.hpp \
						 Circle.cpp \
						 Cross.hpp \
						 Cross.cpp \
						 Rectangle.hpp \
						 Rectangle.cpp \
						 Ring.hpp \
						 Ring.cpp \
						 Image.hpp \
						 Image.cpp \
						 String.hpp \
						 String.cpp \
						 Font.hpp \
						 Font.cpp

libcnbidraw_la_LIBADD = -ldrawtk -lSDL -lcnbicore
libcnbidraw_la_LDFLAGS = $(AM_CFLAGS) \
						-version-info $(CURRENT):$(REVISION):$(AGE)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cnbidraw.pc

