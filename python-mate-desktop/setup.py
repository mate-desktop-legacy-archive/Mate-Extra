#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# setup.py - distutils configuration for pyrsvg


'''Python Bindings for the rsvg library.

PyRsvg is a set of bindings for the rsvg library.'''


import os
import sys
import glob

from distutils.command.build import build
from distutils.core import setup


# Check for windows platform
if sys.platform != 'win32':
    msg =  '*' * 68 + '\n'
    msg += '* Building PyRsvg using distutils is only supported on windows. *\n'
    msg += '* To build PyRsvg in a supported way, read the INSTALL file.    *\n'
    msg += '*' * 68
    raise SystemExit(msg)

# Check for python version
MIN_PYTHON_VERSION = (2, 6, 0)

if sys.version_info[:3] < MIN_PYTHON_VERSION:
    raise SystemExit('ERROR: Python %s or higher is required, %s found.' % (
                         '.'.join(map(str,MIN_PYTHON_VERSION)),
                         '.'.join(map(str,sys.version_info[:3]))))

# Check for pygobject (dsextras)
try:
    from dsextras import GLOBAL_MACROS, GLOBAL_INC, get_m4_define, getoutput, \
                         have_pkgconfig, pkgc_version_check, pkgc_get_defs_dir, \
                         PkgConfigExtension, Template, TemplateExtension, \
                         BuildExt, InstallLib, InstallData
except ImportError:
    raise SystemExit('ERROR: Could not import dsextras module: '
                     'Make sure you have installed pygobject.')

# Check for pkgconfig
if not have_pkgconfig():
    raise SystemExit('ERROR: Could not find pkg-config: '
                     'Please check your PATH environment variable.')


PYGTK_SUFFIX = '2.0'
PYGTK_SUFFIX_LONG = 'gtk-' + PYGTK_SUFFIX
PYGTK_DEFS_DIR = pkgc_get_defs_dir('pygtk-%s' % PYGTK_SUFFIX)

RSVG_REQUIRED      = get_m4_define('rsvg_required_version')
PYCAIRO_REQUIRED   = '1.8.10' # configure.ac does not define pycairo_required_version
PYGTK_REQUIRED     = '%s.%s.%s' % (get_m4_define('pygtk_required_major_version'),
                                   get_m4_define('pygtk_required_minor_version'),
                                   get_m4_define('pygtk_required_micro_version'))

MAJOR_VERSION = int(get_m4_define('mate_python_desktop_major_version'))
MINOR_VERSION = int(get_m4_define('mate_python_desktop_minor_version'))
MICRO_VERSION = int(get_m4_define('mate_python_desktop_micro_version'))
VERSION = '%d.%d.%d' % (MAJOR_VERSION, MINOR_VERSION, MICRO_VERSION)

GLOBAL_INC += ['.']
GLOBAL_MACROS += [('PYRSVG_MAJOR_VERSION', MAJOR_VERSION),
                  ('PYRSVG_MINOR_VERSION', MINOR_VERSION),
                  ('PYRSVG_MICRO_VERSION', MICRO_VERSION),
                  ('VERSION', '\\"%s\\"' % VERSION),
                  ('PLATFORM_WIN32', 1),
                  ('HAVE_BIND_TEXTDOMAIN_CODESET', 1)]

CONFIG_FILE    = 'config.h'
DEFS_DIR       = os.path.join('share', 'pygtk', PYGTK_SUFFIX, 'defs')
HTML_DIR       = os.path.join('share', 'gtk-doc', 'html', 'pyrsvg')


data_files = []
ext_modules = []
py_modules = []
packages = []


class PyRsvgInstallLib(InstallLib):
    def run(self):
        # Modify the base installation dir
        install_dir = os.path.join(self.install_dir, PYGTK_SUFFIX_LONG)
        self.set_install_dir(install_dir)

        InstallLib.run(self)


class PyRsvgInstallData(InstallData):
    def run(self):
        self.add_template_option('VERSION', VERSION)
        self.prepare()

        # Install templates
        self.install_templates()

        InstallData.run(self)

    def install_templates(self):
        self.install_template('pyrsvg-2.0.pc.in',
                              os.path.join(self.install_dir, 'lib','pkgconfig'))


class PyRsvgBuild(build):
    def run(self):
        self.createconfigfile()
        build.run(self)

    def createconfigfile(self):
        with open(CONFIG_FILE, 'w') as fo:
            fo.write ('// Configuration header created by setup.py - do not edit\n' \
                      '#ifndef _CONFIG_H\n' \
                      '#define _CONFIG_H 1\n' \
                      '\n' \
                      '#define PYRSVG_VERSION_MAJOR %s\n' \
                      '#define PYRSVG_VERSION_MINOR %s\n' \
                      '#define PYRSVG_VERSION_MICRO %s\n' \
                      '#define VERSION "%s"\n' \
                      '\n' \
                      '#endif // _CONFIG_H\n' % (MAJOR_VERSION,
                                                 MINOR_VERSION,
                                                 MICRO_VERSION,
                                                 VERSION))


rsvg = TemplateExtension(name='rsvg',
                              pkc_name=('pycairo',
                                        'pygtk-%s' % PYGTK_SUFFIX,
                                        'librsvg-%s' % PYGTK_SUFFIX),
                              pkc_version=(PYCAIRO_REQUIRED,
                                           PYGTK_REQUIRED,
                                           RSVG_REQUIRED),
                              defs='rsvg/rsvg.defs',
                              register=(os.path.join(PYGTK_DEFS_DIR, 'gdk-types.defs').replace('\\', '/'),
                                        os.path.join(PYGTK_DEFS_DIR, 'pango-types.defs').replace('\\', '/'),
                                        os.path.join(PYGTK_DEFS_DIR, 'gtk-types.defs').replace('\\', '/')),
                              override='rsvg/rsvg.override',
                              sources=['rsvg/rsvgmodule.c', 'rsvg/rsvg.c'],
                              py_ssize_t_clean=True)

if rsvg.can_build():
    ext_modules.append(rsvg)
    data_files.append((DEFS_DIR, ('rsvg/rsvg.defs',)))
    data_files.append((HTML_DIR, glob.glob('docs/html/*.html')))
else:
    raise SystemExit('ERROR: Nothing to do, rsvg could not be built and is essential.')

doclines = __doc__.split('\n')
options = {'bdist_wininst': {'install_script': 'pyrsvg_postinstall.py'}}

setup(name='pyrsvg',
      url='http://www.pygtk.org',
      version=VERSION,
      license='LGPL',
      platforms=['MS Windows'],
      maintainer='Gustavo J. A. M. Carneiro',
      maintainer_email='gjc@inescporto.pt',
      description=doclines[0],
      long_description='\n'.join(doclines[2:]),
      provides=['rsvg'],
      requires=['pygtk (>=%s)' % PYGTK_REQUIRED],
      py_modules=py_modules,
      packages=packages,
      ext_modules=ext_modules,
      data_files=data_files,
      scripts=['pyrsvg_postinstall.py'],
      options=options,
      cmdclass={'install_lib': PyRsvgInstallLib,
                'install_data': PyRsvgInstallData,
                'build_ext': BuildExt,
                'build': PyRsvgBuild})
