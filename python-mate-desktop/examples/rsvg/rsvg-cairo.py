#!/usr/bin/env python

import sys
import cairo
import rsvg
import gtk


BORDER_WIDTH = 10


def delete_cb(win, event):
    gtk.main_quit()


def expose_cairo(win, event, svg):

    x, y, w, h = win.allocation
    cr = win.window.cairo_create()
    cr.set_source_color(win.style.fg[win.state])
    cr.rectangle(BORDER_WIDTH, BORDER_WIDTH,
                 w - 2*BORDER_WIDTH, h - 2*BORDER_WIDTH)
    cr.set_line_width(5.0)
    cr.set_line_join(cairo.LINE_JOIN_ROUND)
    cr.stroke()

    if svg != None:
        svg.render_cairo(cr)

    return True

def main():
    win = gtk.Window ()
    win.connect("delete-event", delete_cb)

    svg = None
    if (len (sys.argv) > 1):
        svg = rsvg.Handle(file=sys.argv[1])
    else:
        raise SystemExit("need svg file")

    win.connect("expose-event", expose_cairo, svg)

    print svg.props.width, svg.props.height, svg.props.em, svg.props.ex

    win.show_all()
    win.connect("destroy", lambda w: gtk.main_quit())
    gtk.main()

if __name__ == '__main__':
    main()
