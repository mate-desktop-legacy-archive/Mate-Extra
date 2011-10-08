#! /usr/bin/python
import sys
import pygtk
pygtk.require("2.0")
import gtk
import braseromedia
import braseroburn

if len(sys.argv) < 2:
	print "Usage: write-data file1 file2 ..."
	print "Writes files to a disc or an image."
	sys.exit (1)

# This is absolutely necessary
braseroburn.start ()

# First create a graft point uri (uri/path of an existing file)
# and path (where it should be located on the disc image)
# then add it to a TrackData (here a TrackDataCfg but this is
# done the same way) and finally add the track to a BraseroSession
# (here a BraseroSessionCfg which autoconfigures itself and is
# necessary to work with BurnOptions dialog).
track = braseroburn.TrackDataCfg ()
grafts = []

for arg_num in range (1, len (sys.argv)):
  grafts = grafts + [braseroburn.GraftPoint (sys.argv [arg_num])]
  print "Adding file '" + sys.argv [arg_num] + "'"

track.set_source (grafts, None)
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

