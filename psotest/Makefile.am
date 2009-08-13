INCLUDES = \
	-I$(top_srcdir) \
	-I$(top_builddir) \
	$(PACKAGE_DISPATCHERS_CFLAGS)

dispdir = $(prefix)/libexec/liboptimization-dispatchers-0.1
disp_PROGRAMS = psotest

psotest_SOURCES = \
	psotest.cc \
	Dispatcher/runsolution.cc \
	Dispatcher/extractsolutions.cc

psotest_LDFLAGS = $(PACKAGE_DISPATCHERS_LIBS)

NO_INST_HFILES = \
	Dispatcher/dispatcher.hh \
	Dispatcher/dispatcher.ih

EXTRA_DIST = $(NO_INST_HFILES)
