include Makefile.inc

Makefile.inc: ccResolutions
	ccbuild makefile > Makefile.inc

update:
	ccbuild makefile > Makefile.inc

clean:
	ccbuild clean

INSTALLDIR=$(PACKETEER_INSTALL_PATH)/libexec/liboptimization-dispatchers-0.1

install: all
	install -D psotest "${INSTALLDIR}/psotest"
