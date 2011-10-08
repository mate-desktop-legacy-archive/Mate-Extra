CLEANFILES=
INCLUDES = $(PYTHON_INCLUDES) $(PYGTK_CFLAGS)

defsdir = $(datadir)/pygtk/2.0/defs
defs_DATA = print.defs printui.defs art.defs

EXTRA_DIST = $(defs_DATA) print-arg-types.py

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

common_ldflags = -module -avoid-version

pymatedir = $(pkgpythondir)/mateprint
pymate_PYTHON = __init__.py

pymateexecdir = $(pkgpyexecdir)/mateprint
pymateexec_LTLIBRARIES =

# libmateprint binding
pymateexec_LTLIBRARIES += _print.la
_print_la_CFLAGS = $(LIBMATEPRINT_CFLAGS) $(PYMATEPRINT_DEFS)
_print_la_LDFLAGS = $(common_ldflags) -export-symbols-regex init_print
_print_la_LIBADD = $(LIBMATEPRINT_LIBS)
_print_la_SOURCES = printmodule.c art-gtype.h art-gtype.c
nodist__print_la_SOURCES = print.c
CLEANFILES += print.c
EXTRA_DIST += print.override
print.c: print.defs print.override

# libmateprintui binding
if BUILD_MATEPRINTUI
pymateexec_LTLIBRARIES += ui.la
endif
ui_la_CFLAGS = $(LIBMATEPRINTUI_CFLAGS)
ui_la_LDFLAGS = $(common_ldflags) -export-symbols-regex initui
ui_la_LIBADD = $(LIBMATEPRINTUI_LIBS)
ui_la_SOURCES = printuimodule.c
nodist_ui_la_SOURCES = printui.c
CLEANFILES += printui.c
EXTRA_DIST += printui.override  
printui.c: printui.defs printui.override


# generic rule
.defs.c:
	(cd $(srcdir) \
	 && $(PYGTK_CODEGEN) \
	    $(PYMATEPRINT_DEFS) \
	    --load-types print-arg-types.py \
	    --register $(PYGTK_DEFSDIR)/pango-types.defs \
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --register print.defs \
	    --register $(MATEPYTHON_DEFSDIR)/canvas.defs \
	    --override $*.override \
	    --prefix py$* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c


EXTRA_DIST += wscript

