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

#ifndef __CONDUIT_EVOLUTION_H__
#define __CONDUIT_EVOLUTION_H__

#include <glib.h>
#include <glib/gstring.h>
#include <glib/gtypes.h>
#include <libebook/e-book.h>
#include <libebook/e-vcard.h>
#include <libebook/e-contact.h>
#include <pango/pango.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string.h>

G_BEGIN_DECLS

typedef struct _Hit {
	gchar *text;
	gchar *email;
    gchar *uid;
	GdkPixbuf *pixbuf;
} Hit;

void free_hit (Hit *hit, gpointer unused);

void init (void);

void set_pixbuf_size (int size);

GList * search_sync (const char *query,
                     int         max_results);

G_END_DECLS

#endif /* __CONDUIT_EVOLUTION_H__ */

