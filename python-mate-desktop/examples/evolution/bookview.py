#!/usr/bin/python2.5

import time
import unittest

import evolution.ebook as e
import gobject

class TestBookView(unittest.TestCase):

    def setUp(self):
        self.loop = gobject.MainLoop()
        self.context = self.loop.get_context()
        self.book = e.open_addressbook("default")
        self.contact_id = None # Id of created contacts for cleanup
        self.contacts = None # Contact list from callbacks

    def testCommit(self):
        c1 = e.EContact()
        c1.props.full_name = "Original Name"
        id1 = self.book.add_contact(c1)

        c2 = self.book.get_contact(id1)
        c2.props.full_name = "Modified Name"
        self.book.commit_contact(c2)

        c3 = self.book.get_contact(id1)
        self.assertEqual(c2.props.full_name, c3.props.full_name)

        self.contact_id = id1


    def testContactsAddedCb_NoArgs(self):
        query = e.EBookQuery.field_test(e.CONTACT_FULL_NAME,
                                        e.BOOK_QUERY_CONTAINS,
                                        "Dummy")
        view = self.book.get_book_view(query)

        h1 = view.set_contacts_added_cb(self.added_cb)
        view.start()

        c1 = e.EContact()
        c1.props.full_name = "Dummy Contact"

        self.book.add_contact(c1) # Generates the signal
        self.contact_id = c1.props.id
        self.refresh_gui()

        if (self.contacts):
            self.assertEqual(self.contacts[0].props.full_name,
                             c1.props.full_name)
            self.assertEqual(self.contacts[0].props.id,
                             c1.props.id)
        else:
            raise TypeError("Contacts not updated")

        view.remove_contacts_added_cb(h1)

    def testContactsAddedCb_Args(self):
        extra_data = "Extra Data"
        query = e.EBookQuery.field_test(e.CONTACT_FULL_NAME,
                                        e.BOOK_QUERY_CONTAINS,
                                        "Dummy")

        view = self.book.get_book_view(query)

        h1 = view.set_contacts_added_cb(self.added_cb, extra_data)
        view.start()

        c1 = e.EContact()
        c1.props.full_name = "Dummy Contact"

        self.book.add_contact(c1)
        self.contact_id = c1.props.id
        self.refresh_gui()

        if (self.contacts):
            self.assertEqual(self.contacts[0].props.full_name,
                             c1.props.full_name)
            self.assertEqual(extra_data, self.extra_data)
        else:
            raise TypeError("Contacts not updated")

        view.remove_contacts_added_cb(h1)

    def testContactsChangedCb_NoArgs(self):

        new_name = "Dummy Modified"

        query = e.EBookQuery.field_test(e.CONTACT_FULL_NAME,
                                        e.BOOK_QUERY_CONTAINS,
                                        "Dummy")
        view = self.book.get_book_view(query)

        c1 = e.EContact()
        c1.props.full_name = "Dummy Contact"

        id1 = self.book.add_contact(c1)
        self.contact_id = id1

        h1 = view.set_contacts_changed_cb(self.added_cb)
        view.start()

        c2 = self.book.get_contact(id1)
        c2.props.full_name = new_name
        self.book.commit_contact(c2) # Generates the signal

        self.refresh_gui()

        if (self.contacts):
            self.assertEqual(self.contacts[0].props.full_name,
                             new_name)
            self.assertEqual(self.contacts[0].props.id,
                             c2.props.id)
        else:
            raise TypeError("Contacts not updated")

        view.remove_contacts_changed_cb(h1)

    def testContactsChangedCb_Args(self):
        extra_data = "Extra Data"
        new_name = "Dummy Modified"
        query = e.EBookQuery.field_test(e.CONTACT_FULL_NAME,
                                        e.BOOK_QUERY_CONTAINS,
                                        "Dummy")

        view = self.book.get_book_view(query)

        c1 = e.EContact()
        c1.props.full_name = "Dummy Contact"
        h1 = view.set_contacts_changed_cb(self.added_cb, extra_data)
        view.start()

        id1 = self.book.add_contact(c1)
        self.contact_id = c1.props.id

        c2 = self.book.get_contact(id1)
        c2.props.full_name = new_name

        self.book.commit_contact(c2) # Generates the signal
        self.refresh_gui()

        if (self.contacts):
            self.assertEqual(self.contacts[0].props.full_name,
                             new_name)
            self.assertEqual(extra_data, self.extra_data)
        else:
            raise TypeError("Contacts not updated")

        view.remove_contacts_changed_cb(h1)

    def testGetBookViewWithFields(self):
        query = e.EBookQuery.field_test(e.CONTACT_FULL_NAME,
                                        e.BOOK_QUERY_CONTAINS,
                                        "Dummy")

        print query

        view = self.book.get_book_view(query, [e.CONTACT_NICKNAME])
        view.start()

        print view
        self.refresh_gui()

    def added_cb(self, view, contacts, extra_data=None):
        self.contacts = contacts
        self.extra_data = extra_data

    def tearDown(self):
        if self.contact_id:
            self.book.remove_contact_by_id(self.contact_id)

        self.loop.quit()
        del(self.loop)
        del(self.book)

    def refresh_gui(self, delay=0):
        while self.context.pending():
            self.context.iteration()
        time.sleep(delay)

    def build_view(self, name, query):
        c1 = e.EContact()
        c1.props.full_name = name
        view = self.book.get_book_view(query)

        return (view, c1)


def added_cb(view, contacts, *user_data):
    print view
    print contacts
    print user_data

def main():
    book = e.open_addressbook("default")

    query = e.EBookQuery.field_exists(e.CONTACT_FULL_NAME)

    view = book.get_book_view(query)
    handler = view.set_contacts_added_cb(added_cb)
    h2 = view.set_contacts_added_cb(added_cb, "one")
    h3 = view.set_contacts_removed_cb(added_cb, "removed", "extra_args")
    h4 = view.set_contacts_changed_cb(added_cb, "changed")
    #print view.set_contacts_added_cb(added_cb, None)

    view.remove_contacts_added_cb(handler)

    view.start()

    gtk.main()


if __name__ == "__main__":
    unittest.main()
