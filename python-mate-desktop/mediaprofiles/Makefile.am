AUTOMAKE_OPTIONS=1.7

INCLUDES = $(PYTHON_INCLUDES) $(MEDIAPROFILES_CFLAGS) $(PYGTK_CFLAGS)

EXTRA_DIST = mediaprofiles.defs mediaprofiles.override

defsdir = $(datadir)/pygtk/2.0/defs
defs_DATA = mediaprofiles.defs

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

# mediaprofiles module
pkgpyexec_LTLIBRARIES = mediaprofiles.la
mediaprofiles_la_LDFLAGS = -module -avoid-version -export-symbols-regex initmediaprofiles
mediaprofiles_la_LIBADD = $(MEDIAPROFILES_LIBS)
mediaprofiles_la_SOURCES = mediaprofilesmodule.c
nodist_mediaprofiles_la_SOURCES = mediaprofiles.c
mediaprofiles.c: mediaprofiles.defs mediaprofiles.override
CLEANFILES = mediaprofiles.c

.defs.c:
	(cd $(srcdir)\
	 && $(PYGTK_CODEGEN) \
		--override $*.override \
	    --py_ssize_t-clean \
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --register $(MATEPYTHON_DEFSDIR)/mateconf.defs \
		--prefix py$* $*.defs) > gen-$*.c \
	    && cp gen-$*.c $*.c \
	    && rm -f gen-$*.c

EXTRA_DIST += wscript

