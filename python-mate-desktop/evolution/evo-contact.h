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

#ifndef EVO_CONTACT_H
#define EVO_CONTACT_H

#include "evo-ebook-environment.h"

G_BEGIN_DECLS

char *evo_contact_get_vcard_string(EContact *obj);
char *evo_contact_get_name(EContact *obj);
char *evo_contact_get_uid(EContact *obj);
GdkPixbuf *evo_contact_get_photo (EContact *contact, gint pixbuf_size);

G_END_DECLS

#endif /* EVO_CONTACT_H */
