import sys
# load module beneath
sys.path.append ('..')

import gtk, gobject
gobject.threads_init() # see https://bugzilla.mate.org/show_bug.cgi?id=595342
import braseromedia

# Test listing of drives as they are overriden */
monitor = braseromedia.MediumMonitor ()

media = monitor.get_media (braseromedia.TYPE_ALL)
for medium in media:
  print "Medium " + medium.get_name ()

drives = monitor.get_drives (braseromedia.TYPE_ALL)
for drive in drives:
  print "Drive " + drive.get_display_name ()

s = braseromedia.DriveSelection()

def foo ():
	global s;
	print s.get_active().get_display_name()


s.show()

b = gtk.VBox(False, 0)
b.show()

w = gtk.Window (gtk.WINDOW_TOPLEVEL)
w.add (b)
w.show()

b.add (s)
gobject.idle_add (foo)
gtk.main ()
