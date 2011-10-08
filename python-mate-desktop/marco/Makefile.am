INCLUDES = $(PYTHON_INCLUDES) $(PYGTK_CFLAGS)

defsdir = $(datadir)/pygtk/2.0/defs

defs_DATA = marco.defs

EXTRA_DIST = $(defs_DATA)

common_ldflags = -module -avoid-version

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

# marco binding

if BUILD_MARCO
pkgpyexec_LTLIBRARIES = marco.la
endif
marco_la_CFLAGS = $(MARCO_CFLAGS)
marco_la_LDFLAGS = $(common_ldflags) -export-symbols-regex initmarco
marco_la_LIBADD = $(MARCO_LIBS)
marco_la_SOURCES = marcomodule.c
nodist_marco_la_SOURCES = marco.c
CLEANFILES = marco.c
EXTRA_DIST += marco.override  
marco.c: marco.defs marco.override

.defs.c:
	(cd $(srcdir) \
	 && $(PYGTK_CODEGEN) \
            --register $(PYGTK_DEFSDIR)/gtk-types.defs \
            --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --py_ssize_t-clean \
	    --override $*.override \
	    --prefix py$* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c

EXTRA_DIST += wscript
