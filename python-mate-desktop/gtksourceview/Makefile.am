AUTOMAKE_OPTIONS=1.7

INCLUDES = $(PYTHON_INCLUDES) $(GTKSOURCEVIEW_CFLAGS) $(PYGTK_CFLAGS)

EXTRA_DIST = gtksourceview.defs gtksourceview.override

defsdir = $(datadir)/pygtk/2.0/defs
defs_DATA = gtksourceview.defs

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

# gtksourceview module
pkgpyexec_LTLIBRARIES = gtksourceview.la
gtksourceview_la_LDFLAGS = -module -avoid-version -export-symbols-regex initgtksourceview
gtksourceview_la_LIBADD = $(GTKSOURCEVIEW_LIBS)
gtksourceview_la_SOURCES = gtksourceviewmodule.c 
nodist_gtksourceview_la_SOURCES = gtksourceview.c
gtksourceview.c: gtksourceview.defs gtksourceview.override
CLEANFILES = gtksourceview.c

.defs.c:
	(cd $(srcdir)\
	 && $(PYGTK_CODEGEN) \
		--override $*.override \
	    --py_ssize_t-clean \
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --register $(PYGTK_DEFSDIR)/pango-types.defs \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --register ../mateprint/print.defs \
		--prefix py$* $*.defs) > gen-$*.c \
	    && cp gen-$*.c $*.c \
	    && rm -f gen-$*.c

EXTRA_DIST += wscript
