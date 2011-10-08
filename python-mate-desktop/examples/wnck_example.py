import gtk
import wnck

def main():
    screen = wnck.screen_get_default()
    # Process pending gtk+ events so that wnck can find out about existing windows.
    while gtk.events_pending():
        gtk.main_iteration()
    for window in screen.get_windows():
        # A XID is a number that identifies a X window.
        xid = window.get_xid()
        # By calling gtk.gdk.window_foreign_new(xid), we are creating
        # a gtk.gdk.Window which wraps the window that is identified by
        # the XID. It will return None if it can't find the X window
        # identified by the XID in question.
        wrapped_window = gtk.gdk.window_foreign_new(xid)
        # We are maximizing the window here, but
        # you can do almost anything with a gtk.gdk.Window.
        wrapped_window.maximize()

if __name__ == '__main__':
    main()
