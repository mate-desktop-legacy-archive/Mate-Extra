#!/usr/bin/env python
import pygtk
pygtk.require('2.0')

import gtk
import mateapplet
import gobject

def background_show(applet):
    print "background: ", applet.get_background()

def sample_factory(applet, iid):
    print "Creating new applet instance"
    label = gtk.Label("Success!")
    applet.add(label)
    applet.show_all()
    gobject.timeout_add(1000, background_show, applet)
    return True

print "Starting factory"
mateapplet.matecomponent_factory("OAFIID:MATE_PythonAppletSample_Factory", 
                           mateapplet.Applet.__gtype__, 
                           "hello", "0", sample_factory)
print "Factory ended"

