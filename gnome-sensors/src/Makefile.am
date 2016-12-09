bin_PROGRAMS = gsensors

gsensors_SOURCES = gnome-sensors.c
gsensors_LDADD = -lsensors $(GSENSORS_LIBS)
CC=gcc
CFLAGS = -g -O0

INCLUDES = -I$(top_srcdir) -I$(includedir) $(GSENSORS_CFLAGS) \
			-DGNOMELOCALEDIR=\""$(datadir)/locale"\" \
			-DGNOME_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED \
			-DGDK_DISABLE_DEPRECATED -DG_DISABLE_DEPRECATED
#install-data-am:
#	sed -e 's\FOOMATIC\${prefix}\' GNOME_GSensors.server > g.tmp
#	install -D g.tmp ${prefix}/lib/bonobo/servers/GNOME_GSensors.server
serverdir = $(prefix)/lib/bonobo/servers/
server_DATA = GNOME_GSensors.server
