#! /usr/bin/python
import sys
import pygtk
pygtk.require("2.0")
import gtk
import braseromedia
import braseroburn

if len(sys.argv) != 2:
	print "Usage: write-image image"
	print "Writes an image to a disc."
	sys.exit (1)

# This is absolutely necessary
braseroburn.start ()

track = braseroburn.TrackImageCfg ()
track.set_source (sys.argv [1])
session = braseroburn.SessionCfg ()
session.add_track (track, None)

# Run configure dialog
option_dialog = braseroburn.BurnOptions (session)
res = option_dialog.run ()
option_dialog.destroy ()

if res != gtk.RESPONSE_OK:
 exit ()

# Run BurnDialog and that's it
burn_dialog = braseroburn.BurnDialog ()
burn_dialog.show ()
burn_dialog.run (session)
option_dialog.destroy ()
