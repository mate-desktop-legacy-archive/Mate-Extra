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

#include "evo-contact.h"

/* This removes any vCard attributes that are just "", or have no associated
 * value. Evolution tends to add empty fields like this a lot - as does
 * contacts, to show required fields (but it removes them after editing, via
 * this function).
 * TODO: This really doesn't need to be recursive.
 *
 * Taken from openhand contacts project
 */
void
contacts_clean_contact (EContact *contact)
{
	GList *attributes, *c;

	attributes = e_vcard_get_attributes (E_VCARD (contact));
	for (c = attributes; c; c = c->next) {
		EVCardAttribute *a = (EVCardAttribute*)c->data;
		GList *values = e_vcard_attribute_get_values (a);
		gboolean remove = TRUE;
		for (; values; values = values->next) {
			if (g_utf8_strlen ((const gchar *)values->data, -1) > 0)
				remove = FALSE;
		}
		if (remove) {
			e_vcard_remove_attribute (E_VCARD (contact), a);
			contacts_clean_contact (contact);
			break;
		}
	}
}

gboolean
contacts_contact_is_empty (EContact *contact)
{
	GList *attributes, *c;
	
	attributes = e_vcard_get_attributes (E_VCARD (contact));
	for (c = attributes; c; c = c->next) {
		EVCardAttribute *a = (EVCardAttribute*)c->data;
		GList *values = e_vcard_attribute_get_values (a);
		for (; values; values = values->next) {
			if (g_utf8_strlen ((const gchar *)values->data, -1) > 0)
				return FALSE;
		}
	}
	
	return TRUE;
}


char *
evo_contact_get_vcard_string(EContact *obj)
{
	g_return_val_if_fail(obj != NULL, NULL);
	g_return_val_if_fail(E_IS_CONTACT(obj), NULL);

	EVCard vcard = obj->parent;
	return e_vcard_to_string(&vcard, EVC_FORMAT_VCARD_30);
}

char *
evo_contact_get_name(EContact *obj)
{
	g_return_val_if_fail(obj != NULL, NULL);
	g_return_val_if_fail(E_IS_CONTACT(obj), NULL);
	
	EContactName *name = (EContactName *)e_contact_get(obj, E_CONTACT_NAME);
    if (name)
    	return e_contact_name_to_string (name);
    return NULL;
}

char *
evo_contact_get_uid(EContact *obj)
{
	g_return_val_if_fail(obj != NULL, NULL);
	g_return_val_if_fail(E_IS_CONTACT(obj), NULL);
	
	return  (char *)e_contact_get(obj, E_CONTACT_UID);
}

GdkPixbuf *
evo_contact_get_photo (EContact *obj, gint pixbuf_size)
{
	GdkPixbuf *pixbuf = NULL;
	EContactPhoto *photo = e_contact_get (obj, E_CONTACT_PHOTO);

	if (photo) {
		GdkPixbufLoader *loader;

		loader = gdk_pixbuf_loader_new ();

		if (photo->type == E_CONTACT_PHOTO_TYPE_INLINED) {
			if (gdk_pixbuf_loader_write (loader, (guchar *) photo->data.inlined.data, photo->data.inlined.length, NULL))
				pixbuf = gdk_pixbuf_loader_get_pixbuf (loader);
		}

		if (pixbuf) {
			GdkPixbuf *tmp;
			gint width = gdk_pixbuf_get_width (pixbuf);
			gint height = gdk_pixbuf_get_height (pixbuf);
			double scale = 1.0;

			if (height > width) {
				scale = pixbuf_size / (double) height;
			} else {
				scale = pixbuf_size / (double) width;
			}

			if (scale < 1.0) {
				tmp = gdk_pixbuf_scale_simple (pixbuf, width * scale, height * scale, GDK_INTERP_BILINEAR);
				g_object_unref (pixbuf);
				pixbuf = tmp;
			}
		}
		e_contact_photo_free (photo);
	}
	return pixbuf;
}

