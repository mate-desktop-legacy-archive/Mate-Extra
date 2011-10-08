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

#ifndef EVO_ADDRESSBOOK_H
#define EVO_ADDRESSBOOK_H

#include "evo-ebook-environment.h"

G_BEGIN_DECLS

/* Public */
EBook*    evo_addressbook_open                 (const char  *uri);
GList*    evo_addressbook_get_all_contacts     (EBook       *addressbook);
gboolean  evo_addressbook_get_changed_contacts (EBook       *addressbook,
                                                GList      **added,
                                                GList      **modified,
                                                GList      **deleted,
                                                char        *change_id);
GList*    evo_addressbook_free_text_search     (EBook       *book,
                                                const char  *query);
EContact* evo_addressbook_get_contact          (EBook       *book,
                                                const char  *uid);
gboolean  evo_addressbook_contact_exists       (EBook       *book,
                                                EContact    *contact);
char *    evo_addressbook_add_contact          (EBook       *book,
                                                EContact    *contact);
gboolean  evo_addressbook_remove_contact       (EBook       *book,
                                                EContact    *contact);
char *    evo_addressbook_get_uid              (EBook       *book);

G_END_DECLS

#endif /* EVO_ADDRESSBOOK_H */ 
 
 
