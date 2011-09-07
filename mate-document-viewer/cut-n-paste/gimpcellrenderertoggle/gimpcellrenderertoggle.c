/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimpcellrenderertoggle.c
 * Copyright (C) 2003-2004  Sven Neumann <sven@gimp.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA.
 */

#include <config.h>

#include "gimpwidgetsmarshal.h"
#include "gimpcellrenderertoggle.h"


#define DEFAULT_ICON_SIZE  GTK_ICON_SIZE_BUTTON


enum
{
  CLICKED,
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_STOCK_ID,
  PROP_STOCK_SIZE
};


static void gimp_cell_renderer_toggle_finalize     (GObject         *object);
static void gimp_cell_renderer_toggle_get_property (GObject         *object,
                                                    guint            param_id,
                                                    GValue          *value,
                                                    GParamSpec      *pspec);
static void gimp_cell_renderer_toggle_set_property (GObject         *object,
                                                    guint            param_id,
                                                    const GValue    *value,
                                                    GParamSpec      *pspec);
static void gimp_cell_renderer_toggle_get_size     (GtkCellRenderer *cell,
                                                    GtkWidget       *widget,
                                                    GdkRectangle    *rectangle,
                                                    gint            *x_offset,
                                                    gint            *y_offset,
                                                    gint            *width,
                                                    gint            *height);
static void gimp_cell_renderer_toggle_render       (GtkCellRenderer *cell,
                                                    GdkWindow       *window,
                                                    GtkWidget       *widget,
                                                    GdkRectangle    *background_area,
                                                    GdkRectangle    *cell_area,
                                                    GdkRectangle    *expose_area,
                                                    GtkCellRendererState flags);
static gboolean gimp_cell_renderer_toggle_activate (GtkCellRenderer *cell,
                                                    GdkEvent        *event,
                                                    GtkWidget       *widget,
                                                    const gchar     *path,
                                                    GdkRectangle    *background_area,
                                                    GdkRectangle    *cell_area,
                                                    GtkCellRendererState  flags);
static void gimp_cell_renderer_toggle_create_pixbuf (GimpCellRendererToggle *toggle,
                                                     GtkWidget              *widget);


G_DEFINE_TYPE (GimpCellRendererToggle, gimp_cell_renderer_toggle,
               GTK_TYPE_CELL_RENDERER_TOGGLE)

#define parent_class gimp_cell_renderer_toggle_parent_class

static guint toggle_cell_signals[LAST_SIGNAL] = { 0 };


static void
gimp_cell_renderer_toggle_class_init (GimpCellRendererToggleClass *klass)
{
  GObjectClass         *object_class = G_OBJECT_CLASS (klass);
  GtkCellRendererClass *cell_class   = GTK_CELL_RENDERER_CLASS (klass);

  toggle_cell_signals[CLICKED] =
    g_signal_new ("clicked",
                  G_OBJECT_CLASS_TYPE (object_class),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (GimpCellRendererToggleClass, clicked),
                  NULL, NULL,
                  _gimp_widgets_marshal_VOID__STRING_FLAGS,
                  G_TYPE_NONE, 2,
                  G_TYPE_STRING,
                  GDK_TYPE_MODIFIER_TYPE);

  object_class->finalize     = gimp_cell_renderer_toggle_finalize;
  object_class->get_property = gimp_cell_renderer_toggle_get_property;
  object_class->set_property = gimp_cell_renderer_toggle_set_property;

  cell_class->get_size       = gimp_cell_renderer_toggle_get_size;
  cell_class->render         = gimp_cell_renderer_toggle_render;
  cell_class->activate       = gimp_cell_renderer_toggle_activate;

  g_object_class_install_property (object_class,
                                   PROP_STOCK_ID,
                                   g_param_spec_string ("stock-id",
                                                        NULL, NULL,
                                                        NULL,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT));
  g_object_class_install_property (object_class,
                                   PROP_STOCK_SIZE,
                                   g_param_spec_int ("stock-size",
                                                     NULL, NULL,
                                                     0, G_MAXINT,
                                                     DEFAULT_ICON_SIZE,
                                                     G_PARAM_READWRITE |
                                                     G_PARAM_CONSTRUCT));
}

static void
gimp_cell_renderer_toggle_init (GimpCellRendererToggle *toggle)
{
}

static void
gimp_cell_renderer_toggle_finalize (GObject *object)
{
  GimpCellRendererToggle *toggle = GIMP_CELL_RENDERER_TOGGLE (object);

  if (toggle->stock_id)
    {
      g_free (toggle->stock_id);
      toggle->stock_id = NULL;
    }

  if (toggle->pixbuf)
    {
      g_object_unref (toggle->pixbuf);
      toggle->pixbuf = NULL;
    }

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gimp_cell_renderer_toggle_get_property (GObject    *object,
                                        guint       param_id,
                                        GValue     *value,
                                        GParamSpec *pspec)
{
  GimpCellRendererToggle *toggle = GIMP_CELL_RENDERER_TOGGLE (object);

  switch (param_id)
    {
    case PROP_STOCK_ID:
      g_value_set_string (value, toggle->stock_id);
      break;
    case PROP_STOCK_SIZE:
      g_value_set_int (value, toggle->stock_size);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
      break;
    }
}

static void
gimp_cell_renderer_toggle_set_property (GObject      *object,
                                        guint         param_id,
                                        const GValue *value,
                                        GParamSpec   *pspec)
{
  GimpCellRendererToggle *toggle = GIMP_CELL_RENDERER_TOGGLE (object);

  switch (param_id)
    {
    case PROP_STOCK_ID:
      if (toggle->stock_id)
        g_free (toggle->stock_id);
      toggle->stock_id = g_value_dup_string (value);
      break;
    case PROP_STOCK_SIZE:
      toggle->stock_size = g_value_get_int (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
      break;
    }

  if (toggle->pixbuf)
    {
      g_object_unref (toggle->pixbuf);
      toggle->pixbuf = NULL;
    }
}

static void
gimp_cell_renderer_toggle_get_size (GtkCellRenderer *cell,
                                    GtkWidget       *widget,
                                    GdkRectangle    *cell_area,
                                    gint            *x_offset,
                                    gint            *y_offset,
                                    gint            *width,
                                    gint            *height)
{
  GimpCellRendererToggle *toggle = GIMP_CELL_RENDERER_TOGGLE (cell);
  GtkStyle               *style  = gtk_widget_get_style (widget);
  gint                    calc_width;
  gint                    calc_height;
  gint                    pixbuf_width;
  gint                    pixbuf_height;
  gfloat                  xalign;
  gfloat                  yalign;
  gint                    xpad;
  gint                    ypad;

  if (! toggle->stock_id)
    {
      GTK_CELL_RENDERER_CLASS (parent_class)->get_size (cell,
                                                        widget,
                                                        cell_area,
                                                        x_offset, y_offset,
                                                        width, height);
      return;
    }

  gtk_cell_renderer_get_alignment (cell, &xalign, &yalign);
  gtk_cell_renderer_get_padding (cell, &xpad, &ypad);

  if (! toggle->pixbuf)
    gimp_cell_renderer_toggle_create_pixbuf (toggle, widget);

  pixbuf_width  = gdk_pixbuf_get_width  (toggle->pixbuf);
  pixbuf_height = gdk_pixbuf_get_height (toggle->pixbuf);

  calc_width  = (pixbuf_width +
                 (gint) xpad * 2 + style->xthickness * 2);
  calc_height = (pixbuf_height +
                 (gint) ypad * 2 + style->ythickness * 2);

  if (width)
    *width  = calc_width;

  if (height)
    *height = calc_height;

  if (cell_area)
    {
      if (x_offset)
        {
          *x_offset = (((gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL) ?
                        (1.0 - xalign) : xalign) *
                       (cell_area->width - calc_width));
          *x_offset = MAX (*x_offset, 0);
        }

      if (y_offset)
        {
          *y_offset = yalign * (cell_area->height - calc_height);
          *y_offset = MAX (*y_offset, 0);
        }
    }
}

static void
gimp_cell_renderer_toggle_render (GtkCellRenderer      *cell,
                                  GdkWindow            *window,
                                  GtkWidget            *widget,
                                  GdkRectangle         *background_area,
                                  GdkRectangle         *cell_area,
                                  GdkRectangle         *expose_area,
                                  GtkCellRendererState  flags)
{
  GimpCellRendererToggle *toggle = GIMP_CELL_RENDERER_TOGGLE (cell);
  GtkStyle               *style  = gtk_widget_get_style (widget);
  GdkRectangle            toggle_rect;
  GdkRectangle            draw_rect;
  GtkStateType            state;
  gboolean                active;
  gint                    xpad;
  gint                    ypad;

  if (! toggle->stock_id)
    {
      GTK_CELL_RENDERER_CLASS (parent_class)->render (cell, window, widget,
                                                      background_area,
                                                      cell_area, expose_area,
                                                      flags);
      return;
    }

  gimp_cell_renderer_toggle_get_size (cell, widget, cell_area,
                                      &toggle_rect.x,
                                      &toggle_rect.y,
                                      &toggle_rect.width,
                                      &toggle_rect.height);

  gtk_cell_renderer_get_padding (cell, &xpad, &ypad);
  toggle_rect.x      += cell_area->x + xpad;
  toggle_rect.y      += cell_area->y + ypad;
  toggle_rect.width  -= xpad * 2;
  toggle_rect.height -= ypad * 2;

  if (toggle_rect.width <= 0 || toggle_rect.height <= 0)
    return;

  active =
    gtk_cell_renderer_toggle_get_active (GTK_CELL_RENDERER_TOGGLE (cell));

  if (!gtk_cell_renderer_get_sensitive (cell))
    {
      state = GTK_STATE_INSENSITIVE;
    }
  else if ((flags & GTK_CELL_RENDERER_SELECTED) == GTK_CELL_RENDERER_SELECTED)
    {
      if (gtk_widget_has_focus (widget))
        state = GTK_STATE_SELECTED;
      else
        state = GTK_STATE_ACTIVE;
    }
  else
    {
      if (gtk_cell_renderer_toggle_get_activatable (GTK_CELL_RENDERER_TOGGLE (cell)))
        state = GTK_STATE_NORMAL;
      else
        state = GTK_STATE_INSENSITIVE;
    }

  if (gdk_rectangle_intersect (expose_area, cell_area, &draw_rect) &&
      (flags & GTK_CELL_RENDERER_PRELIT))
    gtk_paint_shadow (style,
                      window,
                      state,
                      active ? GTK_SHADOW_IN : GTK_SHADOW_OUT,
                      &draw_rect,
                      widget, NULL,
                      toggle_rect.x,     toggle_rect.y,
                      toggle_rect.width, toggle_rect.height);

  if (active)
    {
      GdkPixbuf *insensitive = NULL;
      GdkPixbuf *pixbuf = toggle->pixbuf;
      
      toggle_rect.x      += style->xthickness;
      toggle_rect.y      += style->ythickness;
      toggle_rect.width  -= style->xthickness * 2;
      toggle_rect.height -= style->ythickness * 2;

      if (state == GTK_STATE_INSENSITIVE)
        {
	  GtkIconSource *source;

	  source = gtk_icon_source_new ();
	  gtk_icon_source_set_pixbuf (source, pixbuf);
	  /* The size here is arbitrary; since size isn't
	   * wildcarded in the source, it isn't supposed to be
	   * scaled by the engine function
	   */
	  gtk_icon_source_set_size (source, GTK_ICON_SIZE_SMALL_TOOLBAR);
	  gtk_icon_source_set_size_wildcarded (source, FALSE);

	  insensitive = gtk_style_render_icon (gtk_widget_get_style (widget),
					       source,
					       gtk_widget_get_direction (widget),
					       GTK_STATE_INSENSITIVE,
					       /* arbitrary */
					       (GtkIconSize)-1,
					       widget,
					       "gimpcellrenderertoggle");

	  gtk_icon_source_free (source);

	  pixbuf = insensitive;
	}

      if (gdk_rectangle_intersect (&draw_rect, &toggle_rect, &draw_rect))
        {
	  cairo_t *cr;
	  
	  cr = gdk_cairo_create (window);

	  gdk_cairo_set_source_pixbuf (cr, pixbuf, toggle_rect.x, toggle_rect.y);
	  gdk_cairo_rectangle (cr, &draw_rect);
	  cairo_fill (cr);
	  
	  cairo_destroy (cr);
	}
	      
      if (insensitive)
        g_object_unref (insensitive);
    }
}

static gboolean
gimp_cell_renderer_toggle_activate (GtkCellRenderer      *cell,
                                    GdkEvent             *event,
                                    GtkWidget            *widget,
                                    const gchar          *path,
                                    GdkRectangle         *background_area,
                                    GdkRectangle         *cell_area,
                                    GtkCellRendererState  flags)
{
  GtkCellRendererToggle *toggle = GTK_CELL_RENDERER_TOGGLE (cell);

  if (gtk_cell_renderer_toggle_get_activatable (toggle))
    {
      GdkModifierType state = 0;

      GTK_CELL_RENDERER_CLASS (parent_class)->activate (cell, event, widget,
							path, background_area,
							cell_area, flags);

      if (event && ((GdkEventAny *) event)->type == GDK_BUTTON_PRESS)
        state = ((GdkEventButton *) event)->state;

      gimp_cell_renderer_toggle_clicked (GIMP_CELL_RENDERER_TOGGLE (cell),
                                         path, state);

      return TRUE;
    }

  return FALSE;
}

static void
gimp_cell_renderer_toggle_create_pixbuf (GimpCellRendererToggle *toggle,
                                         GtkWidget              *widget)
{
  if (toggle->pixbuf)
    g_object_unref (toggle->pixbuf);

  toggle->pixbuf = gtk_widget_render_icon (widget,
                                           toggle->stock_id,
                                           toggle->stock_size, NULL);
}


/**
 * gimp_cell_renderer_toggle_new:
 * @stock_id: the stock_id of the icon to use for the active state
 *
 * Creates a custom version of the #GtkCellRendererToggle. Instead of
 * showing the standard toggle button, it shows a stock icon if the
 * cell is active and no icon otherwise. This cell renderer is for
 * example used in the Layers treeview to indicate and control the
 * layer's visibility by showing %GIMP_STOCK_VISIBLE.
 *
 * Return value: a new #GimpCellRendererToggle
 *
 * Since: GIMP 2.2
 **/
GtkCellRenderer *
gimp_cell_renderer_toggle_new (const gchar *stock_id)
{
  return g_object_new (GIMP_TYPE_CELL_RENDERER_TOGGLE,
                       "stock_id", stock_id,
                       NULL);
}

/**
 * gimp_cell_renderer_toggle_clicked:
 * @cell: a #GimpCellRendererToggle
 * @path:
 * @state:
 *
 * Emits the "clicked" signal from a #GimpCellRendererToggle.
 *
 * Since: GIMP 2.2
 **/
void
gimp_cell_renderer_toggle_clicked (GimpCellRendererToggle *cell,
                                   const gchar            *path,
                                   GdkModifierType         state)
{
  g_return_if_fail (GIMP_IS_CELL_RENDERER_TOGGLE (cell));
  g_return_if_fail (path != NULL);

  g_signal_emit (cell, toggle_cell_signals[CLICKED], 0, path, state);
}
