INCLUDES = $(PYTHON_INCLUDES)

defsdir = $(datadir)/pygtk/2.0/defs
defs_DATA = caja_burn.defs
EXTRA_DIST = $(defs_DATA)

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

# cajaburn binding
pkgpyexec_LTLIBRARIES = cajaburn.la
cajaburn_la_CFLAGS = $(CAJABURN_CFLAGS)
cajaburn_la_LDFLAGS = -module -avoid-version -export-symbols-regex initcajaburn
cajaburn_la_LIBADD = $(CAJABURN_LIBS)
cajaburn_la_SOURCES = nb_module.c		\
	nb_track.c nb_track.h

nodist_cajaburn_la_SOURCES = caja_burn.c
CLEANFILES = caja_burn.c
EXTRA_DIST += caja_burn.override  
caja_burn.c: caja_burn.defs caja_burn.override

.defs.c:
	(cd $(srcdir) \
	 && $(PYGTK_CODEGEN) \
	    --override $*.override \
	    --py_ssize_t-clean \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --prefix $* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c

EXTRA_DIST += wscript

