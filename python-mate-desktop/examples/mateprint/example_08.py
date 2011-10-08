#! /usr/bin/env python

# *  example_08.c: sample mate-print code
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
import mateprint, mateprint.ui

def my_draw(gpc):
    gpc.beginpage("1")

    gpc.moveto(100, 100)
    gpc.lineto(200, 200)
    gpc.stroke()
    
    gpc.showpage()


def my_print():
    # Create the objects */
    job    = mateprint.Job(mateprint.config_default())
    dialog = mateprint.ui.Dialog(job, "Sample print dialog", 0)
    gpc    = job.get_context()
    config = job.get_config()

    # Run the dialog */
    response = dialog.run()
    if response != mateprint.ui.DIALOG_RESPONSE_PRINT:
	print "Printing was canceled"
	return
    
    # We don't print, we only dump the info 
    my_draw(gpc)
    config.dump()
    job.close()

	
my_print ();
print "Done..."

