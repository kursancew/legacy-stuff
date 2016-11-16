## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = bsplines

bsplines_SOURCES = \
	main.cpp \
	support.cpp support.h \
	interface.cpp interface.h \
	callbacks.cpp callbacks.h

bsplines_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

