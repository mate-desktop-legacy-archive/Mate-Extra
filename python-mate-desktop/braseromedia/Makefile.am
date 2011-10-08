INCLUDES = $(PYTHON_INCLUDES)

defsdir = $(datadir)/pygtk/2.0/defs
defs_DATA = brasero_media.defs
EXTRA_DIST = $(defs_DATA)

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

# braseromedia binding
pkgpyexec_LTLIBRARIES = braseromedia.la
braseromedia_la_CFLAGS = $(BRASEROMEDIA_CFLAGS)
braseromedia_la_LDFLAGS = -module -avoid-version -export-symbols-regex initbraseromedia
braseromedia_la_LIBADD = $(BRASEROMEDIA_LIBS)
braseromedia_la_SOURCES = bm_module.c

nodist_braseromedia_la_SOURCES = brasero_media.c
CLEANFILES = brasero_media.c
EXTRA_DIST += brasero_media.override  
brasero_media.c: brasero_media.defs brasero_media.override

.defs.c:
	(cd $(srcdir) \
	 && $(PYGTK_CODEGEN) \
	    --override $*.override \
	    --py_ssize_t-clean \
	    --register $(PYGOBJECT_DEFSDIR)/gio-types.defs \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --prefix $* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c

EXTRA_DIST += wscript

