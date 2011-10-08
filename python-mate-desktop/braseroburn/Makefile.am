INCLUDES = $(PYTHON_INCLUDES)

defsdir = $(datadir)/pygtk/2.0/defs
defs_DATA = brasero_burn.defs
EXTRA_DIST = $(defs_DATA)

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

# braseroburn binding
pkgpyexec_LTLIBRARIES = braseroburn.la
braseroburn_la_CFLAGS = $(BRASEROBURN_CFLAGS)
braseroburn_la_LDFLAGS = -module -avoid-version -export-symbols-regex initbraseroburn
braseroburn_la_LIBADD = $(BRASEROBURN_LIBS)
braseroburn_la_SOURCES = 		\
		brasero_burn_module.c	\
		brasero-types.h		\
		brasero-types.c

nodist_braseroburn_la_SOURCES = brasero_burn.c
CLEANFILES = brasero_burn.c

LIBBRASERO_BURN_OVERRIDES=	brasero_burn.override

EXTRA_DIST += $(LIBBRASERO_BURN_OVERRIDES)
brasero_burn.c: brasero_burn.defs $(LIBBRASERO_BURN_OVERRIDES)


.defs.c:
	(cd $(srcdir) \
	 && $(PYGTK_CODEGEN) \
	    --override $*.override \
	    --py_ssize_t-clean \
	    --register $(PYGOBJECT_DEFSDIR)/gio-types.defs \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --register $(abs_top_srcdir)/braseromedia/brasero_media.defs \
	    --prefix $* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c

EXTRA_DIST += wscript

