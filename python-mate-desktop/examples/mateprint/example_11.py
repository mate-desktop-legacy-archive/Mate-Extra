#! /usr/bin/env python
#
# *  example_11.c: sample mate-print code
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
# *    Chris Lahey <clahey@ximian.com>
#    Python conversion:
#      Gustavo J. A. M. Carneiro <gustavo@users.sf.net>
# *
# *  Copyright (C) 2002, 2003 Ximian Inc. and authors
# *
# */

#/*
# * See README
# */

import pygtk; pygtk.require("2.0")
import mateprint, mateprint.ui

TEMP_FILE = "temp.ps"

format = "%%! PS-Adobe-3.0 \n"\
	 "1 setlinewidth\n"\
	 "newpath\n"\
	 "100 100 moveto\n"\
	 "600 600 lineto\n"\
	 "stroke\n"\
	 "/Helvetica findfont\n"\
	 "24 scalefont setfont\n"\
	 "100 230 moveto\n"\
	 "(My page size is %fx%f) show\n"\
	 "showpage";


def my_print(job, preview):

	file_ = file(TEMP_FILE, "w")

	config = job.get_config()
	width, height = mateprint.job_get_page_size_from_config(config)

	output = format % (width, height)
	file_.write(output)

	# this is marked with MATE_PRINT_UNSTABLE_API in the public headers
	# OK. So this example becomes useless, I know.
	#job.set_file(TEMP_FILE)

	if not preview:
	    job.print_()
	else:
	    mateprint.ui.JobPreview(job, "Title goes here").show()


job    = mateprint.Job(mateprint.config_default())
dialog = mateprint.ui.Dialog(job, "Sample print dialog", 0)

# Run the dialog
response = dialog.run()
dialog.destroy()
if response == mateprint.ui.DIALOG_RESPONSE_PRINT:
    my_print(job, False)
elif response == mateprint.ui.DIALOG_RESPONSE_PREVIEW:
    my_print(job, True)


