include Makefile.inc

Makefile.inc: ccResolutions
	ccbuild makefile > Makefile.inc

update:
	ccbuild makefile > Makefile.inc

clean:
	ccbuild distclean

INSTALLDIR=$(PACKETEER_INSTALL_PATH)/libexec/liboptimization-dispatcher-0.1/dispatchers

install: all
	install -D webots "${INSTALLDIR}/webots"
