#! /usr/bin/python
import sys
import pygtk
pygtk.require("2.0")
import gtk
import braseromedia
import braseroburn

if len(sys.argv) < 2:
	print "Usage: write-audio song1/video1 song2/video2 ..."
	print "Writes songs to a CD."
	sys.exit (1)

# This is absolutely necessary
braseroburn.start ()

session = braseroburn.SessionCfg ()

for arg_num in range (1, len (sys.argv)):
  track = braseroburn.TrackStreamCfg ()
  track.set_source (sys.argv [arg_num])
  print "Adding '" + sys.argv [arg_num] + "'"
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
