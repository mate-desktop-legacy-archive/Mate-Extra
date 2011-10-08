#! /usr/bin/env python
#
# *  example_09.c: sample mate-print code. This dialog saves the MatePrintConfig
# *                to disk after printing and loads it before creating the dialog
# *                This shows how to implement persistent print configuration.
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

CONFIG_FILE  = "print_config"

def my_config_load_from_file():
    try:
	file_ = file(CONFIG_FILE)
    except IOError:
	print "Config not found"
	return mateprint.config_default()
    return mateprint.config_from_string(file_.read(), 0)

def my_config_save_to_file(config):
    file(CONFIG_FILE, "w").write(config.to_string(0))

	
def my_print():
    config = my_config_load_from_file()
    job = mateprint.Job(config)
    dialog = mateprint.ui.Dialog(job, "Example 09 print dialog", 0)
    response = dialog.run()
    if response == mateprint.ui.DIALOG_RESPONSE_CANCEL:
	print "Printing was canceled, config not saved"
	return
    
    print "Config saved to \"%s\"" % CONFIG_FILE
    my_config_save_to_file(config)

my_print()

print "Done..."

