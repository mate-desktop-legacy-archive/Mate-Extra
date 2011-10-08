
pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0
pytotemdir = $(pkgpythondir)/totem
pytotemexecdir = $(pkgpyexecdir)/totem

pytotemexec_LTLIBRARIES = plparser.la
pytotem_PYTHON = __init__.py

plparser_la_SOURCES = plparsermodule.c
nodist_plparser_la_SOURCES = plparser.c
plparser.c: plparser.defs plparser.override

plparser_la_CFLAGS =				\
	$(TOTEM_PLPARSER_CFLAGS)		\
	$(PYTHON_INCLUDES)

plparser_la_LDFLAGS = -module -avoid-version  -export-symbols-regex initplparser
plparser_la_LIBADD =				\
	$(TOTEM_PLPARSER_LIBS)


.defs.c:
	(cd $(srcdir)					\
	 && $(PYGTK_CODEGEN)				\
	    --override $*.override			\
	    --py_ssize_t-clean \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs	\
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs	\
	    --prefix py$* $*.defs) > gen-$*.c		\
	&& cp gen-$*.c $*.c				\
	&& rm -f gen-$*.c


EXTRA_DIST = plparser.defs plparser.override

CLEANFILES = plparser.c

EXTRA_DIST += wscript
