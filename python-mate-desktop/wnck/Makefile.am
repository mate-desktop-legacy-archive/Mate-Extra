INCLUDES = $(PYTHON_INCLUDES) $(PYGTK_CFLAGS)

defsdir = $(datadir)/pygtk/2.0/defs

defs_DATA = wnck.defs

EXTRA_DIST = $(defs_DATA)

common_ldflags = -module -avoid-version

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

# wnck binding

if BUILD_WNCK
pkgpyexec_LTLIBRARIES = wnck.la
endif
wnck_la_CFLAGS = $(WNCK_CFLAGS) -DWNCK_I_KNOW_THIS_IS_UNSTABLE
wnck_la_LDFLAGS = $(common_ldflags) -export-symbols-regex initwnck
wnck_la_LIBADD = $(WNCK_LIBS)
wnck_la_SOURCES = wnckmodule.c
nodist_wnck_la_SOURCES = wnck.c
CLEANFILES = wnck.c
EXTRA_DIST += wnck.override  
wnck.c: wnck.defs wnck.override

.defs.c:
	(cd $(srcdir) \
	 && $(PYGTK_CODEGEN) \
	    --py_ssize_t-clean \
            --register $(PYGTK_DEFSDIR)/gtk-types.defs \
            --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --override $*.override \
	    --prefix py$* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c

EXTRA_DIST += wscript

