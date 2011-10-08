#! /usr/bin/env python
import pygtk; pygtk.require("2.0")
import gtk
import mediaprofiles

# Fetch existing profiles
profiles = mediaprofiles.get_active_list()

# Dump infos over a profile
def dump(profile):
	print 'Active:', profile.get_active()
	print 'Description:',profile.get_description()
	print 'Extension:',profile.get_extension()
	print 'Id:',profile.get_id()
	print 'Name:',profile.get_name()
	print 'Pipeline:',profile.get_pipeline()
	
# Print info about existing profiles
for profile in profiles:
    print '----------------'
    dump(profile)
    print '----------------\n'
    
# Retreive the profile from an ID
profile = mediaprofiles.lookup("voice")
print '----------------'
print 'Audio profile for ID="voice"'
dump(profile)
print '----------------\n'

# Show a window to allow a profile selection
win = gtk.Window()
# This returns in fact a combo box filled with profiles name,
# so you can use normal combobox methods
chooser = mediaprofiles.chooser_combo()
mediaprofiles.chooser_combo_set_profile(chooser, "voice")

def on_profile_changed(chooser):
	print '----------------'
	dump(mediaprofiles.chooser_combo_get_profile(chooser))
	print '----------------\n'
chooser.connect('changed', on_profile_changed)

win.add(chooser)
win.show_all()

gtk.main()
