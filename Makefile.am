INCLUDES = \
	-I$(top_srcdir) \
	-I$(top_builddir) \
	-I$(srcdir) \
	$(PACKAGE_DISPATCHERS_CFLAGS) \
    -DDATADIR=\""$(datadir)"\" \
    -DLIBDIR=\""$(libdir)"\" \
    -DCONFDIR=\""$(sysconfdir)"\" \
    -DPREFIXDIR=\""$(prefix)"\" \
    -DPKGDATADIR=\""$(pkgdatadir)"\" \
    -DPKGLIBDIR=\""$(pkglibdir)"\"

dispdir = $(prefix)/libexec/liboptimization-dispatchers-0.1
disp_PROGRAMS = webots

webots_SOURCES = \
	webots.cc \
	Dispatcher/destructor.cc \
	Dispatcher/killwebots.cc \
	Dispatcher/mode.cc \
	Dispatcher/ondata.cc \
	Dispatcher/onnewconnection.cc \
	Dispatcher/ontimeout.cc \
	Dispatcher/onwebotskilled.cc \
	Dispatcher/runsolution.cc \
	Dispatcher/timeout.cc \
	Dispatcher/webotspath.cc \
	Dispatcher/world.cc \
	Dispatcher/resolvewebotsexecutable.cc \
	Dispatcher/dispatcher.cc \
	Config/config.cc \
	Config/initialize.cc \
	Config/instance.cc \
	Config/statics.cc

webots_LDFLAGS = $(PACKAGE_DISPATCHERS_LIBS)

NO_INST_HFILES = \
	Dispatcher/dispatcher.hh \
	Dispatcher/dispatcher.ih \
	Config/config.hh \
	Config/config.ih

confexdir = $(datadir)/liboptimization-dispatchers-webots
confex_DATA = webots.conf.ex

EXTRA_DIST = \
	$(NO_INST_HFILES) \
	$(confex_DATA)
