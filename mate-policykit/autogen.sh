#!/bin/sh
# Run this to generate all the initial makefiles, etc.

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

PKG_NAME="PolicyKit-mate"

(test -f $srcdir/src/Makefile.am) || {
    echo -n "**Error**: Directory "\`$srcdir\'" does not look like the"
    echo " top-level $PKG_NAME directory"
    exit 1
}

which mate-autogen.sh || {
    echo "You need to install mate-common from the MATE SVN repository"
    exit 1
}

REQUIRED_AUTOMAKE_VERSION=1.9 USE_MATE2_MACROS=1 USE_COMMON_DOC_BUILD=yes . mate-autogen.sh
