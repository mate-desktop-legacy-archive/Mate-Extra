/* ev-keyring.h
 *  this file is part of evince, a mate document viewer
 *
 * Copyright (C) 2008 Carlos Garcia Campos <carlosgc@mate.org>
 *
 * Evince is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Evince is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __EV_KEYRING_H__
#define __EV_KEYRING_H__

#include <glib.h>
#include <gio/gio.h>

G_BEGIN_DECLS

gboolean ev_keyring_is_available    (void);
gchar   *ev_keyring_lookup_password (const gchar  *uri);
gboolean ev_keyring_save_password   (const gchar  *uri,
				     const gchar  *password,
				     GPasswordSave flags);

G_END_DECLS

#endif /* __EV_KEYRING_H__ */
