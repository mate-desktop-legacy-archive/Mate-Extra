AUTOMAKE_OPTIONS=1.7

INCLUDES = $(PYTHON_INCLUDES) $(RSVG_CFLAGS)

EXTRA_DIST = rsvg.defs rsvg.override

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

# rsvg module
pkgpyexec_LTLIBRARIES = rsvg.la
rsvg_la_LDFLAGS = -module -avoid-version -export-symbols-regex initrsvg
rsvg_la_LIBADD = $(RSVG_LIBS)
rsvg_la_SOURCES = rsvgmodule.c 
nodist_rsvg_la_SOURCES = rsvg.c
rsvg.c: rsvg.defs rsvg.override
CLEANFILES = rsvg.c

.defs.c:
	(cd $(srcdir)\
	 && $(PYGTK_CODEGEN) \
		--override $*.override \
	    --py_ssize_t-clean \
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --register $(PYGTK_DEFSDIR)/pango-types.defs \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
		--prefix py$* $*.defs) > gen-$*.c \
	    && cp gen-$*.c $*.c \
	    && rm -f gen-$*.c


EXTRA_DIST += wscript

