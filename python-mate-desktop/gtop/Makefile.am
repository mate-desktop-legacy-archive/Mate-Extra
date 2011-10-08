
INCLUDES = $(PYTHON_INCLUDES) $(GTOP_CFLAGS)

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

# gtop module
pkgpyexec_LTLIBRARIES = gtop.la
gtop_la_LDFLAGS = -module -avoid-version -export-symbols-regex initgtop
gtop_la_LIBADD = $(GTOP_LIBS)
gtop_la_SOURCES = gtop.c 

EXTRA_DIST = wscript
