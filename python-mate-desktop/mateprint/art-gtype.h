/* -*- Mode: C; c-basic-offset: 4 -*- */
#ifndef __ART_GTYPE_H__
#define __ART_GTYPE_H__

#include <glib-object.h>
#include <libart_lgpl/art_rect.h>
#include <libart_lgpl/art_point.h>
#include <libart_lgpl/art_bpath.h>
#include <libart_lgpl/art_vpath.h>


#define ART_TYPE_DRECT (art_drect_get_type())
#define ART_TYPE_POINT (art_point_get_type())
#define ART_TYPE_BPATH (art_bpath_get_type())
#define ART_TYPE_VPATH (art_vpath_get_type())



G_BEGIN_DECLS


GType     art_drect_get_type   (void) G_GNUC_CONST;
ArtDRect* art_drect_new        (double x0,
			        double y0,
			        double x1,
			        double y1);
GType     art_point_get_type   (void) G_GNUC_CONST;
ArtPoint* art_point_new        (double x,
			        double y);
GType     art_bpath_get_type   (void) G_GNUC_CONST;
ArtBpath* art_bpath_new        (ArtPathcode code,
				double x1,
			        double y1,
				double x2,
			        double y2,
				double x3,
			        double y3);
GType     art_vpath_get_type   (void) G_GNUC_CONST;
ArtVpath* art_vpath_new        (ArtPathcode code,
				double x,
			        double y);

G_END_DECLS


#endif

