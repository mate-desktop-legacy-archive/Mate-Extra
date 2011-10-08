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

/*
 * evo-calendar.c is a wrapper around ECal and ECalComponent allowing
 * access to Evolution Tasks, Memos and Calendar information
 */
 
#include "evo-calendar.h"

ECal *
evo_cal_source_open_source(const char *uri, ECalSourceType type)
{
	ESourceList *sources = NULL;
	ESource *source = NULL;
    ECal *cal = NULL;
	GError *gerror = NULL;

    g_debug("Opening calendar source uri: %s\n", uri);

	if (strcmp(uri, "default")) {
	  	if (!e_cal_get_sources(&sources, type, &gerror)) {
                    g_warning("Unable to get sources for calendar (type %u): %s",
                              type, gerror && gerror->message ? gerror->message : "None");
			g_clear_error(&gerror);
			return NULL;
		}
		
		source = evo_environment_find_source(sources, uri);
		if (!source) {
	  		g_warning("Unable to find source for calendar (type %u)", type);
			return NULL;
		}
		
		cal = e_cal_new(source, type);
		if(!cal) {
	  		g_warning("Failed to create new calendar (type %u)", type);
			return NULL;
		}
		
		if(!e_cal_open(cal, FALSE, &gerror)) {
	  		g_warning("Failed to open calendar (type %u): %s",
                      type, gerror && gerror->message? gerror->message : "None");
			g_object_unref(cal);
			g_clear_error(&gerror);
			return NULL;
		}
	} else {
		if (!e_cal_open_default (&cal, type, NULL, NULL, &gerror)) {
	  		g_warning("Failed to open default calendar: %s",
                      gerror && gerror->message ? gerror->message : "None");
			g_clear_error(&gerror);
			return NULL;
		}
	}
	
	return cal;
}

ECal *
evo_cal_source_open_new_with_absolute_uri(const char *name, const char *uri, ECalSourceType type)
{
    ESource *source = NULL;
    ECal *cal = NULL;
    GError *gerror = NULL;

    g_debug("Opening new calendar source uri: %s\n", uri);
        
    source = e_source_new_with_absolute_uri(name, uri); 

    if (!source) {
        g_warning("Unable to open source for calendar (type %u)", type);
        return NULL;
    }

    cal = e_cal_new(source, type);
    if(!cal) {
        g_warning("Failed to create new calendar (type %u)", type);
        return NULL;
    }

    if(!e_cal_open(cal, FALSE, &gerror)) {
        g_warning("Failed to open calendar (type %u): %s", type,
                  gerror && gerror->message ? gerror->message : "None");
        g_object_unref(cal);
        g_clear_error(&gerror);
        return NULL;
    }

    return cal;
}

gboolean
evo_cal_source_remove_object(ECal *ecal, ECalComponent *obj)
{
    char *uid = NULL;
    GError *error = NULL;
    gboolean ret = FALSE;

    uid = evo_cal_component_get_uid(obj);
    if (uid) {
        if (e_cal_remove_object(ecal, uid, &error)) {
            ret = TRUE;
        } else {
            g_warning("failed to remove %s: %s", uid, error ? error->message : "None");
            g_clear_error(&error);
        }
    }
    g_free(uid);

    return ret;
}

gboolean
evo_cal_source_update_object(ECal *ecal, ECalComponent *obj)
{
	if (!e_cal_modify_object (ecal, e_cal_component_get_icalcomponent(obj), 
				       CALOBJ_MOD_ALL, NULL))
		return FALSE;

    return TRUE;
}


void
evo_cal_source_print_all_objects(ECal *ecal)
{
	GList* ical_objects = NULL;
	GList* l = NULL;
    GError *error = NULL;
    char *str = NULL;

	if (e_cal_get_object_list (ecal, "#t", &ical_objects, &error))
    {
    	for (l = ical_objects; l; l = l->next) {
    		str = icalcomponent_as_ical_string (l->data);
            g_print("%s\n", str);
		}
    
    	e_cal_free_object_list (ical_objects);
	}
}

char *
evo_cal_source_add_object(ECal *ecal, ECalComponent *obj)
{
	char *uid;
    GError *error = NULL;

	//uid = e_cal_component_gen_uid ();
	//e_cal_component_set_uid (comp, uid);
	if (!e_cal_create_object (ecal, e_cal_component_get_icalcomponent (obj), &uid, &error)) {
		g_warning("error adding object: %s\n", error ? error->message : "None");
		g_clear_error (&error);
    }

	return uid;	
}

ECalComponent *
evo_cal_source_get_object(ECal *ecal, const char *uid, const char *rid)
{
	ECalComponent *comp = NULL;
	icalcomponent *icalcomp = NULL;
	GError *error = NULL;

	if (e_cal_get_object (ecal, uid, rid, &icalcomp, &error)) {
		comp = e_cal_component_new ();
		if (!e_cal_component_set_icalcomponent (comp, icalcomp)) {
			g_object_unref (comp);
			icalcomponent_free (icalcomp);
		}
    } else {
		g_warning("Could not find object (uid: %s): %s\n", uid, error ? error->message : "None");
	    g_clear_error (&error);
	}

    return comp;
}

char *
evo_cal_source_get_uid(ECal *ecal)
{
    ESource *source = NULL;
    const char *uid = NULL;
    
    source = e_cal_get_source(ecal);
    if (source)
        uid = e_source_peek_uid(source);
        if (uid)
            return g_strdup(uid);
    return NULL;
}

char *
evo_cal_component_get_uid(ECalComponent *obj)
{
    const char *uid = NULL;
	e_cal_component_get_uid(obj, &uid);

    if (uid)
        return g_strdup(uid);
    else
        return NULL;
}

glong 
evo_cal_component_get_modified(ECalComponent *obj)
{
    struct icaltimetype *tt;

    tt = g_malloc0 (sizeof (struct icaltimetype));

	e_cal_component_get_last_modified(obj, &tt);

    if (tt)
        return icaltime_as_timet(*tt);
    else
        return 0;
}

void
evo_cal_component_set_modified(ECalComponent *obj, glong seconds)
{
    struct icaltimetype tt;

    tt = icaltime_from_timet(seconds, TRUE);
    e_cal_component_set_last_modified(obj, &tt);
}

icalproperty_status
evo_cal_component_get_status(ECalComponent *obj)
{
    icalproperty_status status;

    e_cal_component_get_status(obj, &status);
    return status;
}

void
evo_cal_component_set_status(ECalComponent *obj, icalproperty_status status)
{
    e_cal_component_set_status(obj, status);
}

int
evo_cal_component_get_priority(ECalComponent *comp)
{
    int* pp;
    int p;
  
    e_cal_component_get_priority(comp, &pp);
    if (pp == NULL)
        return 0; // default taken from Ross Burton's tasks
    p = *pp;
    e_cal_component_free_priority(pp);
    return p;
}

void
evo_cal_component_set_priority(ECalComponent *comp, int priority)
{
    if (priority == 0) // 0 means no priority
        e_cal_component_set_priority(comp, NULL);        
    else
        e_cal_component_set_priority(comp, &priority);
}

char *
evo_cal_component_get_url(ECalComponent *obj)
{
    const char* url;

    e_cal_component_get_url(obj, &url);

    if (url == NULL)
      return NULL;

    return g_strdup(url);
}

void 
evo_cal_component_set_url(ECalComponent *obj, const char *url)
{
    e_cal_component_set_url(obj, url);
}


char *
evo_cal_component_get_summary(ECalComponent *obj)
{
    ECalComponentText txt;

    e_cal_component_get_summary(obj, &txt);

    return g_strdup(txt.value);
}

void 
evo_cal_component_set_summary(ECalComponent *obj, const char *summary)
{
    ECalComponentText *txt = NULL;
    
    txt = g_malloc0(sizeof (ECalComponentText));
    txt->value = g_strdup(summary);
    e_cal_component_set_summary(obj, txt);
}

char *
evo_cal_component_get_description(ECalComponent *obj)
{
    GSList *list = NULL;
    char *desc = NULL;
    ECalComponentText *txt = NULL;

    e_cal_component_get_description_list(obj, &list);
    //FIXME: I only return the first description. I should return them all with
    //_wrap_string_glist and friends
    if (list) {
        txt = (ECalComponentText *)list->data;
        if (txt)
            desc = g_strdup(txt->value);
        e_cal_component_free_text_list(list);
    }
    return desc;
}

void
evo_cal_component_set_description(ECalComponent *obj, const char *desc)
{
    GSList l;
	ECalComponentText text;

    if (desc) {
        text.value = g_strdup(desc);
    	text.altrep = NULL;
		l.data = &text;
		l.next = NULL;

		e_cal_component_set_description_list (obj, &l);
    }
}
