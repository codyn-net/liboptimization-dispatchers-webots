INCLUDES = \
	-I$(top_srcdir) \
	-I$(top_builddir) \
	-I$(srcdir) \
	$(PACKAGE_DISPATCHERS_CFLAGS)

dispdir = $(prefix)/libexec/liboptimization-dispatchers-0.1
disp_PROGRAMS = webots

webots_SOURCES = \
	webots.cc \
	Dispatcher/destructor.cc \
	Dispatcher/killwebots.cc \
	Dispatcher/mode.cc \
	Dispatcher/nodisplay.cc \
	Dispatcher/ondata.cc \
	Dispatcher/onnewconnection.cc \
	Dispatcher/ontimeout.cc \
	Dispatcher/onwebotskilled.cc \
	Dispatcher/runsolution.cc \
	Dispatcher/timeout.cc \
	Dispatcher/webotspath.cc \
	Dispatcher/world.cc

webots_LDFLAGS = $(PACKAGE_DISPATCHERS_LIBS)

NO_INST_HFILES = \
	Dispatcher/dispatcher.hh \
	Dispatcher/dispatcher.ih

EXTRA_DIST = $(NO_INST_HFILES)
