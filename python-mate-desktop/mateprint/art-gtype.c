/* -*- Mode: C; c-basic-offset: 4 -*- */
#include <string.h>
#include "art-gtype.h"


static ArtDRect*
_art_drect_copy (ArtDRect *drect)
{
    ArtDRect *new = g_new(ArtDRect, 1);
    art_drect_copy(new, drect);
    return new;
}


GType
art_drect_get_type(void)
{
    static GType type = 0;
	
    if (type == 0)
	type = g_boxed_type_register_static
	    ("ArtDRect",
	     (GBoxedCopyFunc) _art_drect_copy,
	     (GBoxedFreeFunc) g_free);
    return type;
}


ArtDRect *
art_drect_new(double x0, double y0, double x1, double y1)
{
    ArtDRect *drect = g_new(ArtDRect, 1);
    drect->x0 = x0;
    drect->y0 = y0;
    drect->x1 = x1;
    drect->y1 = y1;
    return drect;
}


static ArtPoint*
_art_point_copy (ArtPoint *point)
{
    ArtPoint *new = g_new(ArtPoint, 1);
    new->x = point->x;
    new->y = point->y;
    return new;
}


GType
art_point_get_type(void)
{
    static GType type = 0;
	
    if (type == 0)
	type = g_boxed_type_register_static
	    ("ArtPoint",
	     (GBoxedCopyFunc) _art_point_copy,
	     (GBoxedFreeFunc) g_free);
    return type;
}


ArtPoint *
art_point_new(double x, double y)
{
    ArtPoint *point = g_new(ArtPoint, 1);
    point->x = x;
    point->y = y;
    return point;
}


static ArtBpath*
_art_bpath_copy (ArtBpath *bpath)
{
    ArtBpath *new = g_new(ArtBpath, 1);
    memcpy(new, bpath, sizeof(ArtBpath));
    return new;
}

GType
art_bpath_get_type(void)
{
    static GType type = 0;
	
    if (type == 0)
	type = g_boxed_type_register_static
	    ("ArtBpath",
	     (GBoxedCopyFunc) _art_bpath_copy,
	     (GBoxedFreeFunc) g_free);
    return type;
}

ArtBpath*
art_bpath_new (ArtPathcode code,
	       double x1, double y1,
	       double x2, double y2,
	       double x3, double y3)
{
    ArtBpath *bpath = g_new(ArtBpath, 1);
    bpath->x1 = x1;
    bpath->y1 = y1;
    bpath->x2 = x2;
    bpath->y2 = y2;
    bpath->x3 = x3;
    bpath->y3 = y3;
    return bpath;
}


static ArtVpath*
_art_vpath_copy (ArtVpath *vpath)
{
    ArtVpath *new = g_new(ArtVpath, 1);
    memcpy(new, vpath, sizeof(ArtVpath));
    return new;
}

GType
art_vpath_get_type(void)
{
    static GType type = 0;
	
    if (type == 0)
	type = g_boxed_type_register_static
	    ("ArtVpath",
	     (GBoxedCopyFunc) _art_vpath_copy,
	     (GBoxedFreeFunc) g_free);
    return type;
}

ArtVpath*
art_vpath_new (ArtPathcode code,
	       double x, double y)
{
    ArtVpath *vpath = g_new(ArtVpath, 1);
    vpath->x = x;
    vpath->y = y;
    return vpath;
}

