/*  evolution-python: Python bindings to libecal and libebook
 *  Copyright (c) 2007 John Stowers <john.stowers@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "evo-ebook-environment.h"

GList *
evo_environment_list_addressbooks(void)
{
	GList *paths = NULL;
	ESourceList *sources = NULL;
	ESource *source = NULL;
	gboolean first = FALSE;

	if (!e_book_get_addressbooks(&sources, NULL)) {
		return NULL;
    }

	GSList *g = NULL;
	for (g = e_source_list_peek_groups (sources); g; g = g->next) {
		ESourceGroup *group = E_SOURCE_GROUP (g->data);
		GSList *s = NULL;
		for (s = e_source_group_peek_sources (group); s; s = s->next) {
			source = E_SOURCE (s->data);
			evo_location_t *path = g_malloc0(sizeof(evo_location_t));
			if (!first) {
				first = TRUE;
				path->uri = g_strdup("default");
			} else {
				path->uri = g_strdup(e_source_get_uri(source));
			}
			path->name = g_strdup(e_source_peek_name(source));
			paths = g_list_append(paths, path);
		}
	}
	return paths;
}

ESource *
evo_environment_find_source(ESourceList *list, const char *uri)
{
	GSList *g;
	for (g = e_source_list_peek_groups (list); g; g = g->next) {
		ESourceGroup *group = E_SOURCE_GROUP (g->data);
		GSList *s;
		for (s = e_source_group_peek_sources (group); s; s = s->next) {
			ESource *source = E_SOURCE (s->data);
			if (!strcmp(e_source_get_uri(source), uri))
				return source;
		}
	}
	return NULL;
}

EContact *
evo_environment_get_self_contact(void)
{
    EContact *contact = NULL;
    EBook *book = NULL;
    GError *error = NULL;

    if (!e_book_get_self (&contact, &book, &error)) {
		if (error->code == E_BOOK_ERROR_PROTOCOL_NOT_SUPPORTED) {
			g_warning("There was an error while trying to get the addressbook");
			g_clear_error (&error);
			return NULL;
		}	

		g_clear_error (&error);

		contact = e_contact_new ();

		if (book == NULL) {
			book = e_book_new_system_addressbook (&error);
			if (book == NULL || error != NULL) {
				g_error ("%s\n", error->message);
				g_clear_error (&error);
			}

			if (e_book_open (book, FALSE, NULL) == FALSE) {
				g_warning("Unable to open address book");
				g_clear_error (&error);
			}
		} 
	}

    g_object_unref (book);
    return contact;
}


