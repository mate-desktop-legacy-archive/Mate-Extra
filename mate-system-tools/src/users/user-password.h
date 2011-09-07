/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/* user-password.h: this file is part of users-admin, a mate-system-tools frontend
 * for user administration.
 *
 * Copyright (C) 2010 Milan Bouchet-Valat
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 * Authors: Milan Bouchet-Valat <nalimilan@club.fr>.
 */

#ifndef __USER_PASSWORD_H
#define __USER_PASSWORD_H

#include <gtk/gtk.h>

#define NO_PASSWD_LOGIN_GROUP "nopasswdlogin"


void   on_edit_user_passwd              (GtkButton *button,
                                         gpointer   user_data);


#endif /* USER_PASSWORD_H */
