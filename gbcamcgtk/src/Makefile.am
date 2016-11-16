bin_PROGRAMS=gbcamcgtk
gbcamcgtk_SOURCES=gui.c gui_callbacks.c gui_interface.c gui.h
gbcamcgtk_LDADD=$(GBCAMCGTK_LIBS)
CC=gcc
CFLAGS = -O2

INCLUDES = -I$(top_srcdir) -I$(includedir) $(GBCAMCGTK_CFLAGS) `pkg-config --cflags libgnomeui-2.0`
			-DGNOMELOCALEDIR=\""$(datadir)/locale"\"
#			-DGNOME_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED \
#			-DGDK_DISABLE_DEPRECATED -DG_DISABLE_DEPRECATED
