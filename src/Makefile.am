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

dispdir = $(prefix)/libexec/liboptimization-dispatchers-2.0
disp_PROGRAMS = webots-dispatcher

webots_dispatcher_SOURCES = \
	webots.cc \
	dispatcher.cc \
	config.cc

webots_dispatcher_LDFLAGS = $(PACKAGE_DISPATCHERS_LIBS)

NO_INST_HFILES = \
	dispatcher.hh \
	config.hh

EXTRA_DIST = \
	$(NO_INST_HFILES)

install-exec-hook:
	$(LN_S) -f $(dispdir)/webots-dispatcher $(dispdir)/webots

uninstall-hook:
	$(RM) $(dispdir)/webots
