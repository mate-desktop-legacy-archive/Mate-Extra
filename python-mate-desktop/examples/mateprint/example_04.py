#! /usr/bin/env python
#
# *  example_04.c: sample mate-print code
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
    # Make some UTF-8 strings
    acented = "".join(map(chr, (0xC3, 0xA0, 0xC3, 0xA8, 0xC3, 0xAC,
				0xC3, 0xB2, 0xC3, 0xB9, 0x20, 0xC3,
				0xB1, 0xC3, 0x91, 0x20, 0xC3, 0xBB,
				0xC3, 0xB4, 0x20)))
    
    cyrillic = "".join(map(chr, (0xD0, 0xA1, 0xD0, 0xBE, 0xD0, 0xBC, 0xD0, 0xB5,
				 0x20, 0xD1, 0x80, 0xD0, 0xB0, 0xD0, 0xBD,
				 0xD0, 0xB4, 0xD0, 0xBE, 0xD0, 0xBC, 0x20, 0xD1,
				 0x86, 0xD1, 0x8B, 0xD1, 0x80, 0xD1, 0x83,
				 0xD0, 0xBB, 0xD0, 0xBB, 0xD0, 0xB8, 0xD1, 0x86,
				 0x20, 0xD1, 0x87, 0xD0, 0xB0, 0xD1, 0x80,
				 0xD1, 0x81)))

    # Get this font from:
    #   http://bibliofile.mc.duke.edu/gww/fonts/Unicode.html
    # I used the TTF Caslon Roman.

    font = mateprint.font_find_closest("Caslon Roman", 12)
    font_name = font.get_name()
    print "Found:", font_name
    if font_name != "Caslon Roman":
	print "You might not see cyrillic characters because Caslon Roman was not found.\n"
	
    gpc.beginpage("1")

    gpc.setfont(font)
	
    gpc.moveto(100, 700)
    gpc.show("Some acented characters:")
    gpc.moveto(100, 680)
    gpc.show(acented)

    gpc.moveto(100, 650)
    gpc.show("Some cyrillic:")
    gpc.moveto(100, 630)
    gpc.show(cyrillic)

    gpc.showpage()

def my_print():
    job = mateprint.Job(mateprint.config_default())
    gpc = job.get_context()

    my_draw(gpc)

    job.close()
    job.print_()

my_print()
print "Done..."

