/* Creating a private temporary directory.
   Copyright (C) 2001 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef PARAMS
# if __STDC__ || defined __GNUC__ || defined __SUNPRO_C || defined __cplusplus || __PROTOTYPES
#  define PARAMS(Args) Args
# else
#  define PARAMS(Args) ()
# endif
#endif

#if HAVE_MKDTEMP

/* Get mkdtemp() declaration.  */
#include <stdlib.h>

#else

/* Create a unique temporary directory from TEMPLATE.
   The last six characters of TEMPLATE must be "XXXXXX";
   they are replaced with a string that makes the directory name unique.
   Returns TEMPLATE, or a null pointer if it cannot get a unique name.
   The directory is created mode 700.  */
extern char * mkdtemp PARAMS ((char *template));

#endif
