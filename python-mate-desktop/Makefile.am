
SUBDIRS=

if BUILD_DOCS
  SUBDIRS += docs
endif

if BUILD_MATEPRINT
  SUBDIRS += mateprint
endif

if BUILD_APPLET
  SUBDIRS += mateapplet
endif

if BUILD_GTKSOURCEVIEW
  SUBDIRS += gtksourceview
endif

if BUILD_WNCK
  SUBDIRS += wnck
endif

if BUILD_TOTEM_PLPARSER
  SUBDIRS += totem
endif

if BUILD_GTOP
  SUBDIRS += gtop
endif

if BUILD_CAJABURN
  SUBDIRS += cajaburn
endif

if BUILD_MEDIAPROFILES
  SUBDIRS += mediaprofiles
endif

if BUILD_MARCO
  SUBDIRS += marco
endif

if BUILD_RSVG
  SUBDIRS += rsvg
endif

if BUILD_MATEKEYRING
  SUBDIRS += matekeyring
endif

if BUILD_MATEDESKTOP
  SUBDIRS += matedesktop
endif

if BUILD_EVOLUTION
  SUBDIRS += evolution
endif

if BUILD_EVINCE
  SUBDIRS += evince
endif

if BUILD_BRASEROMEDIA
  SUBDIRS += braseromedia
endif

if BUILD_BRASEROBURN
  SUBDIRS += braseroburn
endif

SUBDIRS +=  tests

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = mate-python-desktop-2.0.pc

## Install the bugbuddy module
pkgpyexecdir = $(pyexecdir)/gtk-2.0
if BUILD_BUGBUDDY
  pkgpyexec_PYTHON = bugbuddy.py
endif


EXTRA_DIST = \
  INSTALL.WAF \
  wscript \
  waf \
  MAINTAINERS \
  COPYING.GPL COPYING.LGPL \
  mate-python-desktop-2.0.pc.in \
  ltihooks.py \
  examples/gtksourceview/test-widget.py \
  examples/applet/applet.py \
  examples/applet/MATE_PythonAppletSample.server \
  examples/applet/README \
  examples/mateprint/test-print.py \
  examples/mateprint/example_01.py \
  examples/mateprint/example_02.py \
  examples/mateprint/example_03.py \
  examples/mateprint/example_04.py \
  examples/mateprint/example_05.py \
  examples/mateprint/example_06.py \
  examples/mateprint/example_08.py \
  examples/mateprint/example_09.py \
  examples/mateprint/example_10.glade \
  examples/mateprint/example_10.py \
  examples/mateprint/example_11.py \
  examples/mateprint/README \
  examples/mateprint/sample-image.png \
  examples/mediaprofiles/profiles.py \
  examples/cajaburn/blank_disc.py \
  examples/cajaburn/drive_selection.py \
  examples/cajaburn/write_iso.py \
  examples/keyring.py \
  examples/keyring-async.py \
  examples/rsvg/rsvg-cairo.py \
  examples/wnck_example.py \
  examples/braseromedia/drive_selection.py \
  examples/braseroburn/write-audio-video.py \
  examples/braseroburn/write-image.py \
  examples/braseroburn/write-data.py


$(srcdir)/ChangeLog:
	-rm -f $(srcdir)/ChangeLog
	$(srcdir)/generate-ChangeLog

dist-hook: dist-hook-local $(srcdir)/ChangeLog

.PHONY : dist-hook-local
