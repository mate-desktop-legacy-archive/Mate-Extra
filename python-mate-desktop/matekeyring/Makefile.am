INCLUDES = $(PYTHON_INCLUDES) $(PYGTK_CFLAGS)

defsdir = $(datadir)/pygtk/2.0/defs

defs_DATA = matekeyring.defs

EXTRA_DIST = $(defs_DATA) matekeyring-argtypes.py

common_ldflags = -module -avoid-version

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

# matekeyring binding

if BUILD_MATEKEYRING
pkgpyexec_LTLIBRARIES = matekeyring.la
endif
matekeyring_la_CFLAGS = $(MATEKEYRING_CFLAGS)
matekeyring_la_LDFLAGS = $(common_ldflags) -export-symbols-regex initmatekeyring
matekeyring_la_LIBADD = $(MATEKEYRING_LIBS)
matekeyring_la_SOURCES = matekeyringmodule.c
nodist_matekeyring_la_SOURCES = matekeyring.c
CLEANFILES = matekeyring.c
EXTRA_DIST += matekeyring.override  
matekeyring.c: matekeyring.defs matekeyring.override matekeyring-argtypes.py

.defs.c:
	(cd $(srcdir) \
	 && $(PYGTK_CODEGEN) \
	    --load-types matekeyring-argtypes.py \
	    --py_ssize_t-clean \
            --register $(PYGTK_DEFSDIR)/gtk-types.defs \
            --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --override $*.override \
	    --prefix py$* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c

EXTRA_DIST += wscript
