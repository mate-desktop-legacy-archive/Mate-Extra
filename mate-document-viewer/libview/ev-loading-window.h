/* ev-loading-window.h
 *  this file is part of evince, a mate document viewer
 *
 * Copyright (C) 2010 Carlos Garcia Campos <carlosgc@mate.org>
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

#ifndef EV_LOADING_WINDOW_H
#define EV_LOADING_WINDOW_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _EvLoadingWindow      EvLoadingWindow;
typedef struct _EvLoadingWindowClass EvLoadingWindowClass;

#define EV_TYPE_LOADING_WINDOW              (ev_loading_window_get_type())
#define EV_LOADING_WINDOW(object)           (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_LOADING_WINDOW, EvLoadingWindow))
#define EV_LOADING_WINDOW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_LOADING_WINDOW, EvLoadingWindowClass))
#define EV_IS_LOADING_WINDOW(object)        (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_LOADING_WINDOW))
#define EV_IS_LOADING_WINDOW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_LOADING_WINDOW))
#define EV_LOADING_WINDOW_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), EV_TYPE_LOADING_WINDOW, EvLoadingWindowClass))

GType      ev_loading_window_get_type (void) G_GNUC_CONST;
GtkWidget *ev_loading_window_new      (GtkWindow       *parent);
void       ev_loading_window_get_size (EvLoadingWindow *window,
				       gint            *width,
				       gint            *height);
void       ev_loading_window_move     (EvLoadingWindow *window,
				       gint             x,
				       gint             y);

G_END_DECLS

#endif /* EV_LOADING_WINDOW_H */
