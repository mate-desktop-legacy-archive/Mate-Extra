/* ev-loading-window.c
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

#include "config.h"

#include <string.h>
#include <glib/gi18n.h>
#include "ev-loading-window.h"

enum {
	PROP_0,
	PROP_PARENT
};

struct _EvLoadingWindow {
	GtkWindow  base_instance;

	GtkWindow *parent;

	gint       x;
	gint       y;
	gint       width;
	gint       height;
};

struct _EvLoadingWindowClass {
	GtkWindowClass base_class;
};

G_DEFINE_TYPE (EvLoadingWindow, ev_loading_window, GTK_TYPE_WINDOW)

static void
ev_loading_window_set_property (GObject      *object,
				guint         prop_id,
				const GValue *value,
				GParamSpec   *pspec)
{
	EvLoadingWindow *window = EV_LOADING_WINDOW (object);

	switch (prop_id) {
	case PROP_PARENT:
		window->parent = g_value_get_object (value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	}
}

static void
ev_loading_window_init (EvLoadingWindow *window)
{
	GtkWindow   *gtk_window = GTK_WINDOW (window);
	GtkWidget   *widget = GTK_WIDGET (window);
	GtkWidget   *hbox;
	GtkWidget   *spinner;
	GtkWidget   *label;
	GtkStyle    *style;
	GdkColor    fg, bg;
	const gchar *loading_text = _("Loading…");
	const gchar *fg_color_name = "info_fg_color";
	const gchar *bg_color_name = "info_bg_color";

	hbox = gtk_hbox_new (FALSE, 12);

	spinner = gtk_spinner_new ();
	gtk_spinner_start (GTK_SPINNER (spinner));
	gtk_box_pack_start (GTK_BOX (hbox), spinner, FALSE, FALSE, 0);
	gtk_widget_show (spinner);

	label = gtk_label_new (loading_text);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	gtk_widget_show (label);

	gtk_container_add (GTK_CONTAINER (window), hbox);
	gtk_widget_show (hbox);

	gtk_widget_set_app_paintable (widget, TRUE);

	gtk_container_set_border_width (GTK_CONTAINER (window), 10);

	gtk_window_set_type_hint (gtk_window, GDK_WINDOW_TYPE_HINT_NOTIFICATION);
	gtk_window_set_accept_focus (gtk_window, FALSE);
	gtk_window_set_decorated (gtk_window, FALSE);
	gtk_window_set_resizable (gtk_window, FALSE);

	style = gtk_widget_get_style (widget);
	if (!gtk_style_lookup_color (style, fg_color_name, &fg) ||
	    !gtk_style_lookup_color (style, bg_color_name, &bg)) {
		fg.pixel = 0;
		fg.red = 0xb800;
		fg.green = 0xad00;
		fg.blue = 0x9d00;

		bg.pixel = 0;
		bg.red = 0xff00;
		bg.green = 0xff00;
		bg.blue = 0xbf00;
	}

	if (!gdk_color_equal (&bg, &style->bg[GTK_STATE_NORMAL]))
		gtk_widget_modify_bg (widget, GTK_STATE_NORMAL, &bg);
	if (!gdk_color_equal (&fg, &style->fg[GTK_STATE_NORMAL]))
		gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, &fg);
}

static GObject *
ev_loading_window_constructor (GType                  type,
			       guint                  n_construct_properties,
			       GObjectConstructParam *construct_params)
{
	GObject         *object;
	EvLoadingWindow *window;
	GtkWindow       *gtk_window;

	object = G_OBJECT_CLASS (ev_loading_window_parent_class)->constructor (type,
									       n_construct_properties,
									       construct_params);
	window = EV_LOADING_WINDOW (object);
	gtk_window = GTK_WINDOW (window);

	gtk_window_set_transient_for (gtk_window, window->parent);
	gtk_window_set_destroy_with_parent (gtk_window, TRUE);

	return object;
}

static void
_cairo_rounded_rectangle (cairo_t *cr,
			  gint     width,
			  gint     height,
			  gdouble  radius)
{
	cairo_move_to (cr, radius, 0);
	cairo_line_to (cr, width - radius, 0);
	cairo_curve_to (cr,
			width, 0,
			width, 0,
			width,
			radius);
	cairo_line_to (cr, width, height - radius);
	cairo_curve_to (cr,
			width,height,
			width, height,
			width - radius,
			height);
	cairo_line_to (cr, radius, height);
	cairo_curve_to (cr,
			0, height,
			0, height,
			0, height - radius);
	cairo_line_to (cr, 0, radius);
	cairo_curve_to (cr,
			0, 0,
			0, 0,
			radius, 0);
}

static void
ev_loading_window_size_allocate (GtkWidget      *widget,
				 GtkAllocation  *allocation)
{
	EvLoadingWindow *window = EV_LOADING_WINDOW (widget);
	GdkPixmap       *mask;
	cairo_t         *cr;
	double           r;

	GTK_WIDGET_CLASS (ev_loading_window_parent_class)->size_allocate (widget, allocation);

	if (allocation->width == window->width && allocation->height == window->height)
		return;

	window->width = allocation->width;
	window->height = allocation->height;

	mask = gdk_pixmap_new (NULL, window->width, window->height, 1);
	cr = gdk_cairo_create (GDK_DRAWABLE (mask));

	cairo_save (cr);
	cairo_rectangle (cr, 0, 0, window->width, window->height);
	cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
	cairo_fill (cr);
	cairo_restore (cr);

	cairo_set_source_rgb (cr, 1., 1., 1.);
	r = MIN (window->width, window->height) / 2.;
	_cairo_rounded_rectangle (cr, window->width, window->height, r);
	cairo_fill (cr);

	cairo_destroy (cr);

	gtk_widget_shape_combine_mask (widget, mask, 0, 0);
	g_object_unref (mask);
}

static void
ev_loading_window_hide (GtkWidget *widget)
{
	EvLoadingWindow *window = EV_LOADING_WINDOW (widget);

	window->x = window->y = 0;

	GTK_WIDGET_CLASS (ev_loading_window_parent_class)->hide (widget);
}

static void
ev_loading_window_class_init (EvLoadingWindowClass *klass)
{
	GObjectClass   *g_object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass *gtk_widget_class = GTK_WIDGET_CLASS (klass);

	g_object_class->constructor = ev_loading_window_constructor;
	g_object_class->set_property = ev_loading_window_set_property;

	gtk_widget_class->size_allocate = ev_loading_window_size_allocate;
	gtk_widget_class->hide = ev_loading_window_hide;

	g_object_class_install_property (g_object_class,
					 PROP_PARENT,
					 g_param_spec_object ("parent",
							      "Parent",
							      "The parent window",
							      GTK_TYPE_WINDOW,
							      G_PARAM_WRITABLE |
							      G_PARAM_CONSTRUCT_ONLY));
}

/* Public methods */
GtkWidget *
ev_loading_window_new (GtkWindow *parent)
{
	GtkWidget *window;

	g_return_val_if_fail (GTK_IS_WINDOW (parent), NULL);

	window = g_object_new (EV_TYPE_LOADING_WINDOW,
			       "parent", parent,
			       NULL);
	return window;
}

void
ev_loading_window_get_size (EvLoadingWindow *window,
			    gint            *width,
			    gint            *height)
{
	if (width) *width = window->width;
	if (height) *height = window->height;
}

void
ev_loading_window_move (EvLoadingWindow *window,
			gint             x,
			gint             y)
{
	if (x == window->x && y == window->y)
		return;

	window->x = x;
	window->y = y;
	gtk_window_move (GTK_WINDOW (window), x, y);
}
