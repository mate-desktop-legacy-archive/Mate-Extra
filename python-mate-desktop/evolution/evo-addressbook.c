/*  evolution-python: Python bindings to libecal and libebook
 *  Copyright (c) 2007 John Stowers <john.stowers@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */
 
#include "evo-addressbook.h"
#include "evo-contact.h"
 
static EContactField search_fields[] = { E_CONTACT_FULL_NAME, E_CONTACT_EMAIL, E_CONTACT_NICKNAME, 0 };
static int n_search_fields = G_N_ELEMENTS (search_fields) - 1;

 /**
 * Split a string of tokens separated by whitespace into an array of tokens.
 *
 * Code taken from deskbar
 */
static GArray *
split_query_string (const gchar *str)
{
	GArray *parts = g_array_sized_new (FALSE, FALSE, sizeof (char *), 2);
	PangoLogAttr *attrs;
	guint str_len = strlen (str), word_start = 0, i;

	attrs = g_new0 (PangoLogAttr, str_len + 1);
	/* TODO: do we need to specify a particular language or is NULL ok? */
	pango_get_log_attrs (str, -1, -1, NULL, attrs, str_len + 1);

	for (i = 0; i < str_len + 1; i++) {
		char *start_word, *end_word, *word;
		if (attrs[i].is_word_end) {
			start_word = g_utf8_offset_to_pointer (str, word_start);
			end_word = g_utf8_offset_to_pointer (str, i);
			word  = g_strndup (start_word, end_word - start_word);
			g_array_append_val (parts, word);
		}
		if (attrs[i].is_word_start) {
			word_start = i;
		}
	}
	g_free (attrs);
	return parts;
}

/**
 * Create a query which looks for the specified string in a contact's full name, email addresses and
 * nick name.
 *
 * Taken from deskbar
 */
static EBookQuery *
create_query (const char* s)
{
	EBookQuery *query;
	GArray *parts = split_query_string (s);
	EBookQuery ***field_queries;
	EBookQuery **q;
	guint j;
	int i;

	q = g_new0 (EBookQuery *, n_search_fields);
	field_queries = g_new0 (EBookQuery **, n_search_fields);

	for (i = 0; i < n_search_fields; i++) {
		field_queries[i] = g_new0 (EBookQuery *, parts->len);
		for (j = 0; j < parts->len; j++) {
			field_queries[i][j] = e_book_query_field_test (search_fields[i], E_BOOK_QUERY_CONTAINS, g_array_index (parts, gchar *, j));
		}
		q[i] = e_book_query_and (parts->len, field_queries[i], TRUE);
	}
	g_array_free (parts, TRUE);

	query = e_book_query_or (n_search_fields, q, TRUE);

	for (i = 0; i < n_search_fields; i++) {
		g_free (field_queries[i]);
	}
	g_free (field_queries);
	g_free (q);

	return query;
}

EBook *
evo_addressbook_open(const char *uri)
{
	GError *gerror = NULL;
	ESourceList *sources = NULL;
	ESource *source = NULL;
	EBook *addressbook = NULL;
	
	g_return_val_if_fail(uri != NULL, FALSE);

	if (strcmp(uri, "default")) {
		if (!e_book_get_addressbooks(&sources, NULL)) {
	  		g_warning("Error getting addressbooks: %s", gerror ? gerror->message : "None");
			g_clear_error(&gerror);
			return NULL;
		}
		
		if (!(source = evo_environment_find_source(sources, uri))) {
			g_warning("Error finding source \"%s\"", uri);
			return NULL;
		}
		
		if (!(addressbook = e_book_new(source, &gerror))) {
			g_warning("Failed to alloc new addressbook: %s", gerror ? gerror->message : "None");
			g_clear_error(&gerror);
			return NULL;
		}
	} else {
		if (!(addressbook = e_book_new_default_addressbook(&gerror))) {
			g_warning("Failed to alloc new default addressbook: %s", gerror ? gerror->message : "None");
			g_clear_error(&gerror);
			return NULL;
		}
	}
	
	if (!e_book_open(addressbook, TRUE, &gerror)) {
		g_warning("Failed to alloc new addressbook: %s", gerror ? gerror->message : "None");
		g_clear_error(&gerror);
		g_object_unref(addressbook);
		return NULL;
	}

    return addressbook;

}

gboolean 
evo_addressbook_get_changed_contacts(EBook *addressbook, GList **added, GList **modified, GList **deleted, char *change_id)
{
    EContact *contact = NULL;
	GList *changes = NULL;
	EBookChange *ebc = NULL;
	GList *l = NULL;
	char *uid = NULL;

    g_return_val_if_fail(change_id != NULL, FALSE);
	
	if (!e_book_get_changes(addressbook, change_id, &changes, NULL)) {
			g_warning("Unable to open changed contacts");
			return FALSE;
	}
	
	for (l = changes; l; l = l->next) 
	{
		ebc = (EBookChange *)l->data;
        contact = (EContact *)ebc->contact;

		uid = e_contact_get(contact, E_CONTACT_UID);
		switch (ebc->change_type) 
		{
			case E_BOOK_CHANGE_CARD_ADDED:
				*added = g_list_prepend(*added, contact);
                g_debug("Added");
				break;
			case E_BOOK_CHANGE_CARD_MODIFIED:
				*modified = g_list_prepend(*modified, contact);
                g_debug("Modified");
				break;
			case E_BOOK_CHANGE_CARD_DELETED:
				*deleted = g_list_prepend(*deleted, contact);
                g_debug("Deleted");
				break;
		}
		g_free(uid);
	}

    g_debug("Changes: %i added, %i modified, %i deleted\n", 
                    g_list_length(*added),
                    g_list_length(*modified),
                    g_list_length(*deleted)
                    );

	return TRUE;
}

/*
 * Note: you may get a message "WARNING **: FIXME: wait for completion unimplemented"
 * if you call search_sync but are not running the gobject main loop.
 * This appears to be harmless: http://bugzilla.mate.org/show_bug.cgi?id=314544
 */
GList *
evo_addressbook_free_text_search(EBook *book, const char *query)
{
	GList *contacts = NULL;

	EBookQuery *book_query = create_query (query);
	e_book_get_contacts (book, book_query, &contacts, NULL);
	e_book_query_unref (book_query);
	return contacts;
}

gboolean
evo_addressbook_contact_exists(EBook *book, EContact *contact)
{
	GError *err = NULL;
	GList *contacts = NULL;
	int found = FALSE;
	EBookQuery *query;
    char *uid = NULL;

	g_return_val_if_fail(contact != NULL, FALSE);

    /* query by uid */
    uid = (char *)e_contact_get(contact, E_CONTACT_UID);
    if (!uid)
        return FALSE;

	query = e_book_query_field_test(E_CONTACT_UID, E_BOOK_QUERY_IS, uid);
	/* ignore errors, but cancellation errors we don't try to go further either */
	if (!e_book_get_contacts(book, query, &contacts, &err)) {
		g_warning("Can't get contacts: %s", err->message);
		g_clear_error(&err);
	}

	if (contacts != NULL) {
		found = TRUE;
		g_list_foreach(contacts, (GFunc)g_object_unref, NULL);
		g_list_free(contacts);
	}

	e_book_query_unref(query);

	return found;
}

gboolean
evo_addressbook_remove_contact(EBook *book, EContact *contact)
{
    int found = FALSE;
    const char *uid = NULL;
    GError *err = NULL;
    
    found = evo_addressbook_contact_exists(book, contact);
    if (found) {
        uid = e_contact_get_const(contact, E_CONTACT_UID);
        if (e_book_remove_contact(book, uid, &err)) {
            return TRUE;
        } else {
    		g_warning("Error removing contact: %s", err->message);
    		g_clear_error(&err);
         }
    }
    return FALSE;
}

EContact *
evo_addressbook_get_contact(EBook *book, const char *uid)
{
    EContact *contact = NULL;
	GError *error = NULL;

	if (!e_book_get_contact (book, uid, &contact, &error)) {
		g_warning("error %d getting card: %s\n", error->code, error->message);
		g_clear_error (&error);
		return NULL;
	}

    return contact;

}

char *
evo_addressbook_add_contact(EBook *book, EContact *contact)
{
	GError *error = NULL;

    if (!e_book_add_contact(book, contact, &error)) {
		g_warning("error adding contact: %s\n", error->message);
		g_clear_error (&error);
		return NULL;
	}
    return evo_contact_get_uid(contact);
}

char *
evo_addressbook_get_uid(EBook *book)
{
    ESource *source = NULL;
    const char *uid = NULL;
    
    source = e_book_get_source(book);
    if (source)
        uid = e_source_peek_uid(source);
        if (uid)
            return g_strdup(uid);
    return NULL;
}
