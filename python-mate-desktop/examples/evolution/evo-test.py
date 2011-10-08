#
# Simple demo application to excercise the evolution python bindings
# (c) 2007 John Stowers <john.stowers@gmail.com>
#

import os

import gobject
import gtk
import gtk.glade
import evolution

DESCRIPTION_IDX = 0
URI_IDX = 1

class Tester:
    def __init__(self):
        path = os.path.join(__file__, "..", "evolution.glade")
        path = os.path.abspath(path)
        self.xml = gtk.glade.XML(path, "GUI")
        self.xml.signal_autoconnect({
                "on_GUI_delete_event" :         self.quit,
                "on_closeButton_clicked" :      self.quit,
                "on_memoAddButton_clicked" :    self._add_memo,
                "on_taskAddButton_clicked" :    self._add_task,
                None : None
                })

        self.addressBook = None
        self.addressBookModel = gtk.ListStore(
                                    gobject.TYPE_STRING,    #UID
                                    gobject.TYPE_STRING,    #Name
                                    gtk.gdk.Pixbuf,         #Image
                                    gobject.TYPE_PYOBJECT,  #Object
                                    )

        self.taskSource = None
        self.taskSourceModel = gtk.ListStore(
                                    gobject.TYPE_STRING,    #UID
                                    gobject.TYPE_STRING,    #Summary
                                    gobject.TYPE_STRING,    #Description
                                    gobject.TYPE_PYOBJECT,  #Object
                                    )
        self.memoSource = None
        self.memoSourceModel = gtk.ListStore(
                                    gobject.TYPE_STRING,    #UID
                                    gobject.TYPE_STRING,    #Summary
                                    gobject.TYPE_STRING,    #Description
                                    gobject.TYPE_PYOBJECT,  #Object
                                    )

        contactCombobox = self.xml.get_widget("contactCombobox")
        self._populate_combobox(evolution.ebook.list_addressbooks(), contactCombobox)
        contactOpenButton = self.xml.get_widget("contactOpenButton")
        contactOpenButton.connect("clicked", self._open_addressbook_clicked, contactCombobox)
        contactTreeview = self.xml.get_widget("contactTreeview")
        self._setup_addressbook_view(contactTreeview)
        contactTreeview.set_model(self.addressBookModel)

        taskCombobox = self.xml.get_widget("taskCombobox")
        self._populate_combobox(evolution.ecal.list_task_sources(), taskCombobox)
        taskOpenButton = self.xml.get_widget("taskOpenButton")
        taskOpenButton.connect("clicked", self._open_task_clicked, taskCombobox)
        taskTreeview = self.xml.get_widget("taskTreeview")
        self._setup_source_view(taskTreeview)
        taskTreeview.set_model(self.taskSourceModel)

        memoCombobox = self.xml.get_widget("memoCombobox")
        self._populate_combobox(evolution.ecal.list_memo_sources(), memoCombobox)
        memoOpenButton = self.xml.get_widget("memoOpenButton")
        memoOpenButton.connect("clicked", self._open_memo_clicked, memoCombobox)
        memoTreeview = self.xml.get_widget("memoTreeview")
        self._setup_source_view(memoTreeview)
        memoTreeview.set_model(self.memoSourceModel)

        self.dialogxml = gtk.glade.XML(path, "AddCalComponentDialog")

    def _setup_addressbook_view(self, view):
        c0 = gtk.TreeViewColumn('Picture', gtk.CellRendererPixbuf(), pixbuf=2)
        view.append_column(c0)

        c1 = gtk.TreeViewColumn('Name', gtk.CellRendererText(), text=1)
        view.append_column(c1)

    def _setup_source_view(self, view):
        view.set_property('enable-grid-lines', True)

        c0 = gtk.TreeViewColumn('Summary', gtk.CellRendererText(), text=1)
        view.append_column(c0)

        c1 = gtk.TreeViewColumn('Description', gtk.CellRendererText(), text=2)
        view.append_column(c1)

    def _open_addressbook_clicked(self, sender, combobox):
        model = combobox.get_model()
        uri = model.get_value(
                        combobox.get_active_iter(),
                        URI_IDX)

        self.addressBook = evolution.ebook.open_addressbook(uri)

        if self.addressBook != None:
            print "Opened Addressbook: %s (uid:%s)" % (uri, self.addressBook.get_uid())
            self.addressBookModel.clear()
            for c in self.addressBook.get_all_contacts():
                self.addressBookModel.append( (c.get_uid(), c.get_name(), c.get_photo(16), c) )
        else:
            print "Error Opening Addressbook: %s" % uri

    def _open_memo_clicked(self, sender, combobox):
        model = combobox.get_model()
        uri = model.get_value(
                        combobox.get_active_iter(),
                        URI_IDX)

        self.memoSource = evolution.ecal.open_calendar_source(uri, evolution.ecal.CAL_SOURCE_TYPE_JOURNAL)
        if self.memoSource != None:
            print "Opened Memo Source: %s (uid:%s)" % (uri, self.memoSource.get_uid())
            self.memoSourceModel.clear()
            for c in self.memoSource.get_all_objects():
                self.memoSourceModel.append( (c.get_uid(), c.get_summary(), c.get_description(), c) )
        else:
            print "Error Opening Memo Source: %s" % uri

    def _open_task_clicked(self, sender, combobox):
        model = combobox.get_model()
        uri = model.get_value(
                        combobox.get_active_iter(),
                        URI_IDX)

        self.taskSource = evolution.ecal.open_calendar_source(uri, evolution.ecal.CAL_SOURCE_TYPE_TODO)
        if self.taskSource != None:
            print "Opened Task Source: %s (uid:%s)" % (uri, self.taskSource.get_uid())
            self.taskSourceModel.clear()
            for c in self.taskSource.get_all_objects():
                self.taskSourceModel.append( (c.get_uid(), c.get_summary(), c.get_description(), c) )
        else:
            print "Error Opening Task Source: %s" % uri

    def _populate_combobox(self, sourceList, comboBox):
        #make a combobox with the addressbooks
        store = gtk.ListStore(gobject.TYPE_STRING,gobject.TYPE_STRING)
        comboBox.set_model(store)

        cell = gtk.CellRendererText()
        comboBox.pack_start(cell, True)
        comboBox.add_attribute(cell, 'text', DESCRIPTION_IDX)

        for name,id in sourceList:
            rowref = store.append( (name, id) )

        comboBox.set_active(0)

    def _add_new_object_to_source(self, objtype, summary, description, source):
        if source == None:
            print "Source must be opened first"
            return

        obj = evolution.ecal.ECalComponent(objtype)
        obj.set_summary(summary)
        obj.set_description(description)
        
        uid = source.add_object(obj)
        print "Added %s to %s (uid: %s)" % (obj, source, uid)


    def _add_memo(self, sender):
        s = self.dialogxml.get_widget("summaryEntry")
        d = self.dialogxml.get_widget("descriptionTextview")
        dlg = self.dialogxml.get_widget("AddCalComponentDialog")
        dlg.set_title("Add Memo")

        response = dlg.run()
        if response == gtk.RESPONSE_OK:
            #get all text from the textview and summary entry
            buf = d.get_buffer()
            desc = buf.get_text(buf.get_start_iter(), buf.get_end_iter(), True)
            summary = s.get_text()

            self._add_new_object_to_source(
                            evolution.ecal.CAL_COMPONENT_JOURNAL,
                            summary, desc,
                            self.memoSource
                            )
        dlg.hide()

    def _add_task(self, sender):
        s = self.dialogxml.get_widget("summaryEntry")
        d = self.dialogxml.get_widget("descriptionTextview")
        dlg = self.dialogxml.get_widget("AddCalComponentDialog")
        dlg.set_title("Add Task")

        response = dlg.run()
        if response == gtk.RESPONSE_OK:
            #get all text from the textview and summary entry
            buf = d.get_buffer()
            desc = buf.get_text(buf.get_start_iter(), buf.get_end_iter(), True)
            summary = s.get_text()

            self._add_new_object_to_source(
                            evolution.ecal.CAL_COMPONENT_TODO,
                            summary, desc,
                            self.taskSource
                            )
        dlg.hide()

    def run(self):
        mainWindow = self.xml.get_widget("GUI")
        mainWindow.set_title("evolution-python v%s.%s.%s" % (
                                    evolution.__version__[0],
                                    evolution.__version__[1],
                                    evolution.__version__[2]
                                    ))
        mainWindow.set_size_request(400,600)
        mainWindow.show_all()
        gtk.main()

    def quit(self, *args):
        gtk.main_quit()

if __name__ == "__main__":
    t = Tester()
    t.run()
