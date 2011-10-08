INCLUDES = $(PYTHON_INCLUDES) $(PYGTK_CFLAGS)

defsdir = $(datadir)/pygtk/2.0/defs

defs_DATA = evince.defs

EXTRA_DIST = $(defs_DATA)

common_ldflags = -module -avoid-version

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

# evince binding

if BUILD_EVINCE
pkgpyexec_LTLIBRARIES = evince.la
endif
evince_la_CFLAGS = $(EVINCE_CFLAGS)
evince_la_LDFLAGS = $(common_ldflags) -export-symbols-regex initevince
evince_la_LIBADD = $(EVINCE_LIBS)
evince_la_SOURCES = evincemodule.c
nodist_evince_la_SOURCES = evince.c
CLEANFILES = evince.c
EXTRA_DIST += evince.override  
evince.c: evince.defs evince.override

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
