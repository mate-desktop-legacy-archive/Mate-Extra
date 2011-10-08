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

#ifndef EVO_ENVIRONMENT_H
#define EVO_ENVIRONMENT_H

#include <stdio.h>
#include <string.h>

#include <glib.h>
#include <glib-object.h>

#include <libebook/e-book.h>
#include <libebook/e-vcard.h>

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <pango/pango.h>

G_BEGIN_DECLS

typedef struct evo2_location {
	char *name;
	char *uri;
} evo_location_t;

typedef enum evo_contact_change_type {
	CHANGE_ADDED,
	CHANGE_MODIFIED,
	CHANGE_DELETED
} evo_change_t;

GList *evo_environment_list_addressbooks(void);
ESource *evo_environment_find_source(ESourceList *list, const char *uri);
EContact *evo_environment_get_self_contact(void);

G_END_DECLS

#endif /* EVO_ENVIRONMENT_H */
