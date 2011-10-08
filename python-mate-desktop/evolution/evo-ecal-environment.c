/*  evolution-python: Python bindings to libecal and libebook
 *  Copyright (c) 2007 John Stowers <john.stowers@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "evo-ecal-environment.h"

GList *
evo_environment_list_cal_sources(ECalSourceType type)
{
    GList *paths = NULL;
	ESourceList *sources = NULL;
	ESource *source = NULL;
	
	if (!e_cal_get_sources(&sources, type, NULL)) {
		return NULL;
	}

	GSList *g = NULL;
	for (g = e_source_list_peek_groups (sources); g; g = g->next) {
		ESourceGroup *group = E_SOURCE_GROUP (g->data);
		GSList *s = NULL;
		for (s = e_source_group_peek_sources (group); s; s = s->next) {
			source = E_SOURCE (s->data);
			evo_location_t *path = g_malloc0(sizeof(evo_location_t));
            //FIXME: Word around #442187
			//if (!first) {
			//	first = TRUE;
			//	path->uri = g_strdup("default");
			//} else {
				path->uri = g_strdup(e_source_get_uri(source));
			//}
			path->name = g_strdup(e_source_peek_name(source));
			paths = g_list_append(paths, path);
		}
	}
	return paths;
}

ESource *
evo_environment_find_source(ESourceList *list, const char *uri)
{
	GSList *g;
	for (g = e_source_list_peek_groups (list); g; g = g->next) {
		ESourceGroup *group = E_SOURCE_GROUP (g->data);
		GSList *s;
		for (s = e_source_group_peek_sources (group); s; s = s->next) {
			ESource *source = E_SOURCE (s->data);
			if (!strcmp(e_source_get_uri(source), uri))
				return source;
		}
	}
	return NULL;
}


