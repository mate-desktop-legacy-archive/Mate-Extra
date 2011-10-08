import sys
sys.path.append ('..')
import cajaburn
import gtk
import gobject

r = cajaburn.Recorder()
drives = cajaburn.get_drives_list(True)
d = drives[0]
print r.blank_disc (d, cajaburn.RECORDER_BLANK_FAST, False)
print "done"
