-*- mode: autoconf -*-
AC_PREREQ(2.52)

dnl the mate-python version number
m4_define(mate_python_desktop_major_version, 2)
m4_define(mate_python_desktop_minor_version, 32)
m4_define(mate_python_desktop_micro_version, 1)
m4_define(mate_python_desktop_version, mate_python_desktop_major_version.mate_python_desktop_minor_version.mate_python_desktop_micro_version)

dnl required versions of other packages
m4_define(pygtk_required_major_version, 2)
m4_define(pygtk_required_minor_version, 10)
m4_define(pygtk_required_micro_version, 3)
m4_define(pygtk_required_version, pygtk_required_major_version.pygtk_required_minor_version.pygtk_required_micro_version)
m4_define(glib_required_version,              2.6.0)
m4_define(gtk_required_version,               2.4.0)
m4_define(mate_python_required_version,      2.10.0)
m4_define(libmatepanelapplet_required_version,    2.13.4)
m4_define(libmateprint_required_version,     2.2.0)
m4_define(libmateprintui_required_version,   2.2.0)
m4_define(gtksourceview_required_version,     1.1.90)
m4_define(wnck_required_version,              2.19.3)
m4_define(libgtop_required_version,           2.13.0)
m4_define(libcaja_burn_required_version,  2.15.3)
m4_define(libbrasero_media_required_version,  2.29)
m4_define(mediaprofiles_required_version,     2.10.0)
m4_define(mateconf_required_version,             2.10.0)
m4_define(marco_required_version,          2.21.5)
m4_define(rsvg_required_version,              2.13.93)
m4_define(matekeyring_required_version,      0.5.0)
m4_define(matedesktop_required_version,      2.10.0)
m4_define(totem_required_version,             1.4.0)
m4_define(libecal_required_version,           1.4.0)
m4_define(libebook_required_version,          1.4.0)

AC_INIT(mate-python-desktop, mate_python_desktop_version,
        [http://bugzilla.mate.org/enter_bug.cgi?product=mate-python])
dnl AC_CONFIG_SRCDIR([mate/uimodule.c])
AM_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE

AC_DEFINE(MATE_PYTHON_DESKTOP_MAJOR_VERSION, mate_python_desktop_major_version,
                                            [Mate-Python-Desktop major version])
AC_DEFINE(MATE_PYTHON_DESKTOP_MINOR_VERSION, mate_python_desktop_minor_version,
                                            [Mate-Python-Desktop minor version])
AC_DEFINE(MATE_PYTHON_DESKTOP_MICRO_VERSION, mate_python_desktop_micro_version,
                                            [Mate-Python-Desktop macro version])

dnl AC_ARG_BINDING(NAME, DEFAULT [, MODULENAME])
AC_DEFUN([AC_ARG_BINDING],
[AC_ARG_ENABLE([$1],
             AC_HELP_STRING([--ifelse([$2], [YES], dis, en)able-[$1]],
                            [build ifelse([$3], , [$1], [$3]) bindings (default is [$2])]),
             [if test "$enable_[$1]" = "yes"; then 
	     	build_[$1]=true
	      else
	      	build_[$1]=false
	      fi])
])

MATE_COMMON_INIT

AC_DISABLE_STATIC
AC_PROG_LIBTOOL
dnl when using libtool 2.x create libtool early, because it's used in configure
m4_ifdef([LT_OUTPUT], [LT_OUTPUT])


dnl check for python
AM_PATH_PYTHON(2.3)
AM_CHECK_PYTHON_HEADERS(,[AC_MSG_ERROR(could not find Python headers)])

dnl check for pygtk
PKG_CHECK_MODULES(PYGTK, pygtk-2.0 >= pygtk_required_version)
AC_SUBST(PYGTK_CFLAGS)
AC_PATH_PROG(PYGTK_CODEGEN, pygtk-codegen-2.0, no)
if test "x$PYGTK_CODEGEN" = xno; then
  AC_MSG_ERROR(could not find pygtk-codegen-2.0 script)
fi

AC_MSG_CHECKING([for conflicting pygtk versions])
pygtk_version=`$PKG_CONFIG --modversion pygtk-2.0`
case $pygtk_version in
     2.9.0|2.9.1)
        AC_MSG_RESULT([found $pygtk_version])
        AC_MSG_ERROR([invalid pygtk version found; please upgrade])
        ;;
     *)
        AC_MSG_RESULT([none])
        ;;
esac

AC_MSG_CHECKING(for pygobject defs)
PYGOBJECT_DEFSDIR=`$PKG_CONFIG --variable=defsdir pygobject-2.0`
AC_SUBST(PYGOBJECT_DEFSDIR)
AC_MSG_RESULT($PYGOBJECT_DEFSDIR)

AC_MSG_CHECKING(for pygtk defs)
PYGTK_DEFSDIR=`$PKG_CONFIG --variable=defsdir pygtk-2.0`
AC_SUBST(PYGTK_DEFSDIR)
AC_MSG_RESULT($PYGTK_DEFSDIR)

AC_MSG_CHECKING(for mate-python defs)
MATEPYTHON_DEFSDIR=`$PKG_CONFIG --variable=defsdir mate-python-2.0`
AC_SUBST(MATEPYTHON_DEFSDIR)
AC_MSG_RESULT($MATEPYTHON_DEFSDIR)

AC_MSG_CHECKING(for mate-python argtypes dir)
MATE_PYTHON_ARGTYPES_DIR=`$PKG_CONFIG --variable=argtypesdir mate-python-2.0`
AC_SUBST(MATE_PYTHON_ARGTYPES_DIR)
AC_MSG_RESULT($MATE_PYTHON_ARGTYPES_DIR)

dnl get rid of the -export-dynamic stuff from the configure flags ...
export_dynamic=`(./libtool --config; echo eval echo \\$export_dynamic_flag_spec) | sh`

dnl check for glib
AM_PATH_GLIB_2_0(glib_required_version,,[AC_MSG_ERROR(maybe you want the gtk-mate-1-2 branch?)],$extras_mods)
if test -n "$export_dynamic"; then
  GLIB_LIBS=`echo $GLIB_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl check to make sure we can find gtk
AM_PATH_GTK_2_0(gtk_required_version,,[AC_MSG_ERROR(maybe you want the gtk-mate-1-2 branch?)],$extras_mods)
if test -n "$export_dynamic"; then
  GTK_LIBS=`echo $GTK_LIBS | sed -e "s/$export_dynamic//"`
fi


AC_ARG_ENABLE([gtk-doc],
             AC_HELP_STRING([--disable-gtk-doc],
                            [Disables gtk-doc documentation generation]),,
		[enable_gtk_doc="yes"])
AM_CONDITIONAL(BUILD_DOCS, [test "$enable_gtk_doc" = "yes"])

dnl default bindings to build:
default_bindings_on="applet mateprint mateprintui gtksourceview wnck totem_plparser gtop cajaburn mediaprofiles rsvg matekeyring matedesktop bugbuddy marco evolution evolution_ecal evince braseromedia braseroburn"
default_bindings_off="marco"
default_bindings_all="$default_bindings_on $default_bindings_off"

AC_ARG_ENABLE([allbindings],
             AC_HELP_STRING([--disable-allbindings],
                            [Changes all the bindings default values to be either enabled or disabled, overridden by any explicit bindings on the commandline.]),
             [if test "$enable_allbindings" = "no"; then
	     	default_bindings_off="$default_bindings_all"
		default_bindings_on=""
	      else
	     	default_bindings_off=""
		default_bindings_on="$default_bindings_all"
	      fi])
for binding in $default_bindings_on; do
	eval build_$binding=true
done
for binding in $default_bindings_off; do
	eval build_$binding=false
done

dnl should we build the mate.applet module?
AC_ARG_BINDING([applet],[YES],[mate.applet])
if $build_applet; then
	PKG_CHECK_MODULES(LIBPANELAPPLET, [libmatepanelapplet-2.0 >= libmatepanelapplet_required_version
					   mate-python-2.0 >= mate_python_required_version
                                           libmate-2.0 libmateui-2.0],
	  build_applet=true,
	  build_applet=false)
fi
AC_SUBST(LIBPANELAPPLET_CFLAGS)
AC_SUBST(LIBPANELAPPLET_LIBS)
AM_CONDITIONAL(BUILD_APPLET, $build_applet)
if test -n "$export_dynamic"; then
  LIBPANELAPPLET_LIBS=`echo $LIBPANELAPPLET_LIBS | sed -e "s/$export_dynamic//"`
fi




dnl should we build the mateprint module?
AC_ARG_BINDING([mateprint],[YES])
if $build_mateprint; then
	PKG_CHECK_MODULES(LIBMATEPRINT, [libmateprint-2.2 >= libmateprint_required_version
					 mate-python-2.0 >= mate_python_required_version],
	  build_mateprint=true,
	  build_mateprint=false)
fi
AC_SUBST(LIBMATEPRINT_CFLAGS)
AC_SUBST(LIBMATEPRINT_LIBS)
AM_CONDITIONAL(BUILD_MATEPRINT, $build_mateprint)
if test -n "$export_dynamic"; then
  LIBMATEPRINT_LIBS=`echo $LIBMATEPRINT_LIBS | sed -e "s/$export_dynamic//"`
fi

if [ $build_mateprint = true ]; then
   AC_MSG_CHECKING([for libmateprint 2.8 API])
   if $PKG_CONFIG --modversion "libmateprint-2.2 >= 2.7.2" > /dev/null 2>&1; then
      AC_MSG_RESULT([found])
      PYMATEPRINT_DEFS="-DHAVE_MATEPRINT28";
   else
      AC_MSG_RESULT([not found])
   fi
fi
AC_SUBST(PYMATEPRINT_DEFS)

dnl should we build the mateprint.ui module?
AC_ARG_BINDING([mateprintui],[YES],[mateprint.ui])
if $build_mateprintui; then
	PKG_CHECK_MODULES(LIBMATEPRINTUI, [libmateprintui-2.2 >= libmateprintui_required_version,
					   mate-python-2.0 >= mate_python_required_version],
	  build_mateprintui=true,
	  build_mateprintui=false)
fi
AC_SUBST(LIBMATEPRINTUI_CFLAGS)
AC_SUBST(LIBMATEPRINTUI_LIBS)
AM_CONDITIONAL(BUILD_MATEPRINTUI, $build_mateprintui)
if test -n "$export_dynamic"; then
  LIBMATEPRINTUI_LIBS=`echo $LIBMATEPRINTUI_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl should we build the gtksourceview module?
AC_ARG_BINDING([gtksourceview],[YES])
if $build_gtksourceview; then
	PKG_CHECK_MODULES(GTKSOURCEVIEW, gtksourceview-1.0 >= gtksourceview_required_version,
	  build_gtksourceview=true,
	  build_gtksourceview=false)
fi
AM_CONDITIONAL(BUILD_GTKSOURCEVIEW, $build_gtksourceview)
if test -n "$export_dynamic"; then
  GTKSOURCEVIEW_LIBS=`echo $GTKSOURCEVIEW_LIBS | sed -e "s/$export_dynamic//"`
fi


dnl should we build the wnck module
AC_ARG_BINDING([wnck],[YES])
if $build_wnck; then
	PKG_CHECK_MODULES(WNCK, libwnck-1.0 >= wnck_required_version,
	  build_wnck=true,
	  build_wnck=false)
fi
AM_CONDITIONAL(BUILD_WNCK, $build_wnck)
if test -n "$export_dynamic"; then
  WNCK_LIBS=`echo $WNCK_LIBS | sed -e "s/$export_dynamic//"`
fi



dnl should we build the totem.plparser module?
AC_ARG_BINDING([totem_plparser],[YES],[totem.plparser])
if $build_totem_plparser; then
	PKG_CHECK_MODULES(TOTEM_PLPARSER, [totem-plparser >= totem_required_version  pygtk-2.0 >= pygtk_required_version],
	  build_totem_plparser=true,
	  build_totem_plparser=false)
fi
AM_CONDITIONAL(BUILD_TOTEM_PLPARSER, $build_totem_plparser)
if test -n "$export_dynamic"; then
  TOTEM_PLPARSER_LIBS=`echo $TOTEM_PLPARSER_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl should we build the gtop module?
AC_ARG_BINDING([gtop],[YES])
if $build_gtop; then
	PKG_CHECK_MODULES(GTOP, [libgtop-2.0 >= libgtop_required_version pygtk-2.0 >= pygtk_required_version],
	  build_gtop=true,
	  build_gtop=false)
fi
AM_CONDITIONAL(BUILD_GTOP, $build_gtop)
if test -n "$export_dynamic"; then
  GTOP_LIBS=`echo $GTOP_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl should we build the cajaburn module?
AC_ARG_BINDING([cajaburn],[YES])
if $build_cajaburn; then
	PKG_CHECK_MODULES(CAJABURN, [libcaja-burn >= libcaja_burn_required_version
					 pygtk-2.0 >= pygtk_required_version],
	  build_cajaburn=true,
	  build_cajaburn=false)
fi
AM_CONDITIONAL(BUILD_CAJABURN, $build_cajaburn)
if test -n "$export_dynamic"; then
  CAJABURN_LIBS=`echo $CAJABURN_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl should we build the braseromedia module?
AC_ARG_BINDING([braseromedia],[YES])
if $build_braseromedia; then
	PKG_CHECK_MODULES(BRASEROMEDIA, [libbrasero-media >= libbrasero_media_required_version
					 pygtk-2.0 >= pygtk_required_version],
	  build_braseromedia=true,
	  build_braseromedia=false)
fi
AM_CONDITIONAL(BUILD_BRASEROMEDIA, $build_braseromedia)
if test -n "$export_dynamic"; then
  BRASEROMEDIA_LIBS=`echo $BRASEROMEDIA_LIBS | sed -e "s/$export_dynamic//"`
fi


dnl should we build the braseroburn module?
AC_ARG_BINDING([braseroburn],[YES])
if $build_braseroburn; then
	PKG_CHECK_MODULES(BRASEROBURN, [libbrasero-burn >= 2.29
					 pygtk-2.0 >= pygtk_required_version],
	  build_braseroburn=true,
	  build_braseroburn=false)
fi
AM_CONDITIONAL(BUILD_BRASEROBURN, $build_braseroburn)
if test -n "$export_dynamic"; then
  BRASEROBURN_LIBS=`echo $BRASEROBURN_LIBS | sed -e "s/$export_dynamic//"`
fi


dnl should we build the mediaprofiles module?
AC_ARG_BINDING([mediaprofiles],[YES])
if $build_mediaprofiles; then
	PKG_CHECK_MODULES(MEDIAPROFILES, [mate-media-profiles >= mediaprofiles_required_version
				   pygtk-2.0 >= pygtk_required_version
				   mateconf-2.0 >= mateconf_required_version],
	  build_mediaprofiles=true,
	  build_mediaprofiles=false)
fi
AM_CONDITIONAL(BUILD_MEDIAPROFILES, $build_mediaprofiles)
if test -n "$export_dynamic"; then
  MEDIAPROFILES_LIBS=`echo $MEDIAPROFILES_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl should we build the rsvg module?
AC_ARG_BINDING([rsvg],[YES])
if $build_rsvg; then
	PKG_CHECK_MODULES(RSVG, [librsvg-2.0 >= rsvg_required_version
				 pycairo
				 pygtk-2.0 >= pygtk_required_version],
	  build_rsvg=true,
	  build_rsvg=false)
fi
AM_CONDITIONAL(BUILD_RSVG, $build_rsvg)
if test -n "$export_dynamic"; then
  RSVG_LIBS=`echo $RSVG_LIBS | sed -e "s/$export_dynamic//"`
fi


dnl should we build the marco module
AC_ARG_BINDING([marco],[NO])
if $build_marco; then
    PKG_CHECK_MODULES(MARCO, libmarco-private >= marco_required_version,
      build_marco=true,
      build_marco=false)
    if test -n "$export_dynamic"; then
      MARCO_LIBS=`echo $MARCO_LIBS | sed -e "s/$export_dynamic//"`
    fi
fi
AM_CONDITIONAL(BUILD_MARCO, $build_marco)

dnl should we build the matekeyring module
AC_ARG_BINDING([matekeyring],[YES])
if $build_matekeyring; then
	PKG_CHECK_MODULES(MATEKEYRING, [mate-keyring-1 >= matekeyring_required_version
					 gobject-2.0 >= glib_required_version],
	  build_matekeyring=true,
	  build_matekeyring=false)
fi
AM_CONDITIONAL(BUILD_MATEKEYRING, $build_matekeyring)
if test -n "$export_dynamic"; then
  MATEKEYRING_LIBS=`echo $MATEKEYRING_LIBS | sed -e "s/$export_dynamic//"`
fi

if $PKG_CONFIG --exists "mate-keyring-1 >= 2.20.1"; then
  AC_DEFINE(HAVE_KEYRING_2_20_1, 1, [defined if matekeyring 2.20.1 API is found])
fi

dnl should we build the matedesktop module
AC_ARG_BINDING([matedesktop],[YES])
if $build_matedesktop; then
	PKG_CHECK_MODULES(MATEDESKTOP, [mate-desktop-2.0 >= matedesktop_required_version
					 pygobject-2.0],
	  build_matedesktop=true,
	  build_matedesktop=false)
fi
AM_CONDITIONAL(BUILD_MATEDESKTOP, $build_matedesktop)
if test -n "$export_dynamic"; then
  MATEDESKTOP_LIBS=`echo $MATEDESKTOP_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl Check for bug-buddy
AC_ARG_BINDING([bugbuddy],[YES])
if $build_bugbuddy; then
	AC_PATH_PROG(BUG_BUDDY, bug-buddy)
	if test "x$BUG_BUDDY" != "x"; then 
	  AC_MSG_CHECKING([whether bug-buddy >= 2.16])
	  if $BUG_BUDDY --version | $PYTHON -c ["import sys; sys.exit([int(s) for s in sys.stdin.read().split()[-1].split('.')] < [2,16])"]; then
	    AC_MSG_RESULT([yes])
	    build_bugbuddy=true
	  else
	    AC_MSG_RESULT([no])
	    build_bugbuddy=false
	  fi
	fi
fi
AM_CONDITIONAL(BUILD_BUGBUDDY, $build_bugbuddy)

dnl should we build the evolution module
AC_ARG_BINDING([evolution],[YES])

dnl can also build without ecal support (desirable for maemo)
AC_ARG_BINDING([evolution_ecal],[YES],[Evolution ECal])

if $build_evolution; then
    PKG_CHECK_MODULES(EVOLUTION_EBOOK, [libebook-1.2 >= libebook_required_version
                                        pygobject-2.0
                                        gdk-pixbuf-2.0
                                        pango],
    build_evolution=true,
    build_evolution=false)
    if test -n "$export_dynamic"; then
        EVOLUTION_EBOOK_LIBS=`echo $EVOLUTION_EBOOK_LIBS | sed -e "s/$export_dynamic//"`
    fi
    
    if $build_evolution_ecal; then
        PKG_CHECK_MODULES(EVOLUTION_ECAL,  [libecal-1.2 >= libecal_required_version
                                            pygobject-2.0
                                            gdk-pixbuf-2.0
                                            pango],
        build_evolution_ecal=true,
        build_evolution_ecal=false)
        if test -n "$export_dynamic"; then
            EVOLUTION_ECAL_LIBS=`echo $EVOLUTION_ECAL_LIBS | sed -e "s/$export_dynamic//"`
        fi
    fi
else
    if $build_evolution_ecal; then
      AC_MSG_ERROR(You must enable the Evolution module as well if you want the Evolution Cal module)
    fi
fi
AM_CONDITIONAL(BUILD_EVOLUTION, $build_evolution)
AM_CONDITIONAL(BUILD_EVOLUTION_ECAL, $build_evolution_ecal)

dnl should we build the evince module
AC_ARG_BINDING([evince],[YES])
if $build_evince; then
    PKG_CHECK_MODULES(EVINCE, [evince-document-2.32 evince-view-2.32],
      build_evince=true,
      build_evince=false)
    if test -n "$export_dynamic"; then
      EVINCE_LIBS=`echo $EVINCE_LIBS | sed -e "s/$export_dynamic//"`
    fi
fi
AM_CONDITIONAL(BUILD_EVINCE, $build_evince)


dnl add required cflags ...
JH_ADD_CFLAG([-Wall])
JH_ADD_CFLAG([-std=c9x])
JH_ADD_CFLAG([-fno-strict-aliasing])

AC_CONFIG_FILES(
  Makefile
  mateapplet/Makefile
  mateprint/Makefile
  gtksourceview/Makefile
  wnck/Makefile
  totem/Makefile
  gtop/Makefile
  cajaburn/Makefile
  braseromedia/Makefile
  braseroburn/Makefile
  mediaprofiles/Makefile
  marco/Makefile
  rsvg/Makefile
  matekeyring/Makefile
  matedesktop/Makefile
  evolution/Makefile
  evince/Makefile
  mate-python-desktop-2.0.pc
  tests/Makefile
  docs/Makefile
  docs/gtksourceview/Makefile
  docs/mateprint/Makefile
  docs/mateprintui/Makefile
)
AC_OUTPUT

echo
echo "The following modules will be built:"
echo
$build_applet            && $have_pymatecorba && echo     mateapplet
$build_mateprint        && echo     mateprint
$build_mateprintui      && echo     mateprint.ui
$build_gtksourceview     && echo     gtksourceview
$build_wnck              && echo     wnck
$build_totem_plparser    && echo     totem.plparser
$build_gtop              && echo     gtop
$build_cajaburn      && echo     cajaburn
$build_braseromedia      && echo     braseromedia
$build_braseroburn       && echo     braseroburn
$build_mediaprofiles     && echo     mediaprofiles
$build_marco          && echo     marco
$build_rsvg              && echo     rsvg
$build_matekeyring      && echo     matekeyring
$build_matedesktop      && echo     matedesktop
$build_bugbuddy          && echo     bugbuddy
$build_evolution         && echo     evolution
$build_evolution_ecal    && echo     evolution ecal support
$build_evince            && echo     evince
echo

echo "The following modules will NOT be built:"
echo
$build_applet            && $have_pymatecorba || echo     mateapplet
$build_mateprint        || echo     mateprint
$build_mateprintui      || echo     mateprint.ui
$build_gtksourceview     || echo     gtksourceview
$build_wnck              || echo     wnck
$build_totem_plparser    || echo     totem.plparser
$build_gtop              || echo     gtop
$build_cajaburn      || echo     cajaburn
$build_braseromedia      || echo     braseromedia
$build_braseroburn       || echo     braseroburn
$build_mediaprofiles     || echo     mediaprofiles
$build_marco          || echo     marco
$build_rsvg              || echo     rsvg
$build_matekeyring      || echo     matekeyring
$build_matedesktop      || echo     matedesktop
$build_bugbuddy          || echo     bugbuddy
$build_evolution         || echo     evolution
$build_evolution_ecal    || echo     evolution ecal support
$build_evince            || echo     evince


echo "*****************************************************************************"
echo "*** WARNING: You are using a deprecated build system (autotools)."
echo "***          You should be using WAF instead; see the file INSTALL.WAF."
echo "***          The autotools build system will be removed in a future version."
echo "*****************************************************************************"
