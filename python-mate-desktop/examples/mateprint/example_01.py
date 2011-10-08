#! /usr/bin/env python
#
# *  example_01.c: sample mate-print code
# *
# *  This program is free software; you can redistribute it and/or
# *  modify it under the terms of the GNU Library General Public License
# *  as published by the Free Software Foundation; either version 2 of
# *  the License, or (at your option) any later version.
# *
# *  This program is distributed in the hope that it will be useful,
# *  but WITHOUT ANY WARRANTY; without even the implied warranty of
# *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# *  GNU Library General Public License for more details.
# *
# *  You should have received a copy of the GNU Library General Public
# *  License along with this program; if not, write to the Free Software
# *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
# *
# *  Authors:
# *    Chema Celorio <chema@ximian.com>
#    Python conversion:
#      Gustavo J. A. M. Carneiro <gustavo@users.sf.net>
# *
# *  Copyright (C) 2002 Ximian Inc. and authors
# *
# */

#/*
# * See README
# */
import pygtk; pygtk.require("2.0")
import mateprint

def my_draw(gpc):
    gpc.beginpage("1")

    gpc.moveto(100, 100)
    gpc.lineto(200, 200)
    gpc.stroke()
	
    gpc.showpage()

def my_print():
    job = mateprint.Job(mateprint.config_default())
    gpc = job.get_context()

    my_draw(gpc)

    job.close()
    job.print_()


my_print()
print "Done..."

