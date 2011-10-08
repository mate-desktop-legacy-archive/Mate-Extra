INCLUDES = $(PYTHON_INCLUDES)

defsdir = $(datadir)/pygtk/2.0/defs
defs_DATA = _matedesktop.defs

EXTRA_DIST = $(defs_DATA)

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0


_matedesktopdir = $(pkgpyexecdir)/matedesktop
_matedesktop_LTLIBRARIES = _matedesktop.la
_matedesktop_la_LDFLAGS = -module -avoid-version -export-symbols-regex init_matedesktop
_matedesktop_la_LIBADD = $(MATEDESKTOP_LIBS)
_matedesktop_la_CFLAGS = $(MATEDESKTOP_CFLAGS)
_matedesktop_la_SOURCES = \
	_matedesktopmodule.c 

nodist__matedesktop_la_SOURCES = _matedesktop.c

_matedesktop_PYTHON = \
	__init__.py

CLEANFILES = _matedesktop.c
EXTRA_DIST += _matedesktop.override

_matedesktop.c: _matedesktop.defs _matedesktop.override
.defs.c:
	(cd $(srcdir) \
	 && $(PYGTK_CODEGEN) \
	    --override $*.override \
	    --py_ssize_t-clean \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --prefix py$* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c

EXTRA_DIST += wscript
