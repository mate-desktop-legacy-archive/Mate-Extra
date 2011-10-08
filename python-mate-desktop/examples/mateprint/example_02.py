#! /usr/bin/env python
#/*
# *  example_02.c: sample mate-print code
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
import gtk.gdk
import struct


NUMBER_OF_PIXELS=256

def my_print_image_from_pixbuf(gpc, pixbuf):
    raw_image = pixbuf.get_pixels()
    has_alpha = pixbuf.get_has_alpha()
    rowstride = pixbuf.get_rowstride()
    height    = pixbuf.get_height()
    width     = pixbuf.get_width()
	
    if has_alpha:
	gpc.rgbaimage(raw_image, width, height, rowstride)
    else:
	gpc.rgbimage(raw_image, width, height, rowstride)


def my_print_image_from_disk(gpc):
    # Load the image into a pixbuf
    pixbuf = gtk.gdk.pixbuf_new_from_file("sample-image.png")
    # Save the graphic context, scale, print the image and restore
    gpc.gsave()
    gpc.scale(144, 144)
    my_print_image_from_pixbuf(gpc, pixbuf)
    gpc.grestore()


def my_print_image_from_memory(gpc):
    pixels = NUMBER_OF_PIXELS;

    # Create the image in memory
    color_image = []
    for y in xrange(pixels):
	for x in  xrange(pixels):
	    color_image.append(struct.pack("BBB",
					   (x + y) >> 1,
					   (x + (pixels - 1  - y)) >> 1,
					   ((pixels - 1 - x) + y) >> 1))
    # All images in postscript are printed on a 1 x 1 square, since we
    # want an image which has a size of 2" by 2" inches, we have to scale
    # the CTM (Current Transformation Matrix). Save the graphic state and
    # restore it after we are done so that our scaling does not affect the
    # drawing calls that follow.
    gpc.gsave()
    gpc.scale(144, 144)
    gpc.rgbimage("".join(color_image), pixels, pixels, pixels * 3)
    gpc.grestore()


def my_draw(gpc):
    gpc.beginpage("1")

    gpc.translate(200, 100)
    my_print_image_from_memory(gpc)

    gpc.translate(0, 150)
    my_print_image_from_disk(gpc)
	
    gpc.showpage()


def my_print():
    job = mateprint.Job(mateprint.config_default())
    gpc = job.get_context()

    my_draw(gpc)

    job.close()
    job.print_()


my_print()
print "Done..."

