#! /usr/bin/env python

# *  example_10.c: sample mate-print code
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
import gobject, gtk
import gtk.glade as glade
import sys

class MyDoc(object): __slots__ = ('view', 'config', 'name')

class MyApp(object):
    __slots__ = ('status_bar', 'active_doc', 'doc1', 'doc2', 'doc3')


app = MyApp()

def my_status_bar_pop(id):
    app.status_bar.remove(0, id)
    return False


def my_status_bar_print(message):
    num = app.status_bar.push(0, message)
    gobject.timeout_add(3000, my_status_bar_pop, num)


def my_print_image_from_pixbuf(gpc, pixbuf):
    raw_image = pixbuf.get_pixels()
    has_alpha = pixbuf.get_has_alpha()
    rowstride = pixbuf.get_rowstride()
    height    = pixbuf.get_height()
    width     = pixbuf.get_width()
	
    if has_alpha:
	gpc.rgbaimage(raw_image, width, height, rowstride)
    else:
	gpc.rgbimage(raw_image, width, height, rowstride)


def my_print_image_from_disk(gpc):
    pixbuf = gtk.gdk.pixbuf_new_from_file("sample-image.png")

    gpc.gsave()
    gpc.scale(144, 144)
    my_print_image_from_pixbuf(gpc, pixbuf)
    
    gpc.grestore()


def my_draw(gpc, page):
    font = mateprint.font_find_closest("Sans Regular", 18)

    gpc.beginpage(str(page))

    gpc.moveto(1, 1)
    gpc.lineto(200, 200)
    gpc.stroke()

    gpc.setfont(font)
    gpc.moveto(200, 72)
    gpc.show("Page: %d" % (page + 1))

    my_print_image_from_disk(gpc)
    gpc.showpage()


def my_print(job, preview):
    gpc = job.get_context()
    for i in xrange(4):
	my_draw(gpc, i)

    job.close()

    if not preview:
	my_status_bar_print("Printing ...")
	job.print_()
    else:
	my_status_bar_print("Print previewing ...")
	mateprint.ui.JobPreview(job, "Title goes here").show()


def my_print_cb(widget):
    # Create the objects
    job    = mateprint.Job(app.active_doc.config)
    dialog = mateprint.ui.Dialog(job, "Sample print dialog",
				  mateprint.ui.DIALOG_RANGE | mateprint.ui.DIALOG_COPIES)
    dialog.construct_range_page(mateprint.ui.RANGE_ALL|mateprint.ui.RANGE_SELECTION,
				1, 2, "A", "Lines")

    response = dialog.run()

    if response == mateprint.ui.DIALOG_RESPONSE_PRINT:
	my_print(job, False)

    elif response == mateprint.ui.DIALOG_RESPONSE_PREVIEW:
	my_print(job, True)


def my_print_preview_cb(widget):
    job = mateprint.Job(app.active_doc.config)
    my_print(job, True)


def my_print_setup_cb(widget):

    notebook = gtk.Notebook()

    # MatePrintPaperSelector */
    paper_selector = mateprint.ui.PaperSelector(app.active_doc.config, 0)
    paper_selector.set_size_request(200, 400)
    label = gobject.new(gtk.Label, label="P_aper", use_underline=True)
    notebook.append_page(paper_selector, label)

    # MatePrintPaperSelector */
    paper_selector = mateprint.ui.PaperSelector(app.active_doc.config, 0)
    paper_selector.set_size_request(200, 400)
    label = gobject.new(gtk.Label, label="_foo", use_underline=True)
    notebook.append_page(paper_selector, label)


    # Dialog
    dialog = gtk.Dialog("Printer Setup", None, 0,
			buttons=(gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL,
				 gtk.STOCK_OK,     gtk.RESPONSE_OK))
    dialog.vbox.pack_start(notebook, True, True, 0)
    dialog.show_all()
	
    response = dialog.run()
    dialog.destroy()


def my_font_dialog_cb(widget):
    font = None
    dialog = mateprint.ui.FontDialog("Sample Font dialog")
    fontsel = dialog.get_fontsel()
    if font is not None:
	fontsel.set_font(font)

    dialog.show_all()
    dialog.run()

    font = fontsel.get_font()

    dialog.destroy()


# This function is called when a document is activated. We set the
# active document here and update the MatePrintWidgets with the
# active document config
def my_document_activate_cb(view, doc):
    if app.active_doc is doc:
	return
    app.active_doc = doc
    my_status_bar_print("The active document is now \"%s\"" % doc.name)


# Creates a new document
# 
# Return Value: a pointer to the new document, aborts on error
def my_new_doc(doc_name, gui):
	doc = MyDoc()
	doc.view = gui.get_widget(doc_name)
	doc.name = doc_name
	doc.config = mateprint.config_default()
	doc.view.connect("grab_focus", my_document_activate_cb, doc)

	return doc


def my_app_load():
    gui = glade.XML("example_10.glade")

    # Connect menu items to callbacks
    item = gui.get_widget("menu_quit_item")
    item.connect("activate", lambda arg1: gtk.main_quit())

    item = gui.get_widget("window1")
    item.connect("delete_event", lambda arg1, arg2: gtk.main_quit())

    item = gui.get_widget("menu_print_item")
    item.connect("activate", my_print_cb)
    
    item = gui.get_widget("menu_print_preview_item")
    item.connect("activate", my_print_preview_cb)

    item = gui.get_widget("menu_print_setup_item")
    item.connect("activate", my_print_setup_cb)

#     item = gui.get_widget("menu_tree_item")
#     item.connect("activate", my_tree_cb)

    item = gui.get_widget("menu_font_item")
    item.connect("activate", my_font_dialog_cb)

    app.status_bar = gui.get_widget("statusbar")
    app.doc1 = my_new_doc("doc1", gui)
    if 0:
	app.doc2 = my_new_doc("doc2", gui)
	app.doc3 = my_new_doc("doc3", gui)
    app.active_doc = None
    app.doc1.view.grab_focus()
    
    return True

# --- main ---

if  not my_app_load():
    sys.exit(-1)

gtk.main()

