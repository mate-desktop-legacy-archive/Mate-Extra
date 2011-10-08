INCLUDES = $(PYTHON_INCLUDES)

defsdir = $(datadir)/pygtk/2.0/defs

defs_DATA = ebook.defs

if BUILD_EVOLUTION_ECAL
	defs_DATA += ecal.defs
endif

pkgpythondir = $(pyexecdir)/gtk-2.0/evolution
pkgpyexecdir = $(pyexecdir)/gtk-2.0/evolution

pkgpyexec_LTLIBRARIES = ebook.la
pkgpyexec_DATA = __init__.py

ebook_la_LDFLAGS = -module -avoid-version -export-symbols-regex initebook
ebook_la_CFLAGS = $(EVOLUTION_EBOOK_CFLAGS)
ebook_la_LIBADD = $(EVOLUTION_EBOOK_LIBS)
ebook_la_SOURCES = \
	ebookmodule.c \
	override_common.c \
	evo-ebook-environment.c \
	evo-contact.c \
	evo-addressbook.c \
	ebook-enums.c \
	override_common.h \
	evo-ebook-environment.h \
	evo-contact.h \
	evo-addressbook.h \
	ebook-enums.h

nodist_ebook_la_SOURCES = ebook.c

if BUILD_EVOLUTION_ECAL
    pkgpyexec_LTLIBRARIES += ecal.la
endif

ecal_la_LDFLAGS = -module -avoid-version -export-symbols-regex initecal
ecal_la_CFLAGS = $(EVOLUTION_ECAL_CFLAGS)
ecal_la_LIBADD = $(EVOLUTION_ECAL_LIBS)
ecal_la_SOURCES = \
	ecalmodule.c \
	override_common.c \
	evo-ecal-environment.c \
	evo-calendar.c \
	override_common.h \
	evo-ecal-environment.h \
	evo-calendar.h

nodist_ecal_la_SOURCES = ecal.c

CLEANFILES = ebook.c ecal.c

EXTRA_DIST = \
	ebook.defs \
	ecal.defs \
	ebook.override \
	ecal.override \
	__init__.py

ebook.c: ebook.defs ebook.override
ecal.c: ecal.defs ecal.override
.defs.c:
	(cd $(srcdir) \
	 && $(PYGTK_CODEGEN) \
	    --override $*.override \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gdk-base-types.defs \
	    --prefix py$* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c

EXTRA_DIST += wscript
