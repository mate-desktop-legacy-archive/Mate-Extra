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

#ifndef EVO_CALENDAR_H
#define EVO_CALENDAR_H

#include "evo-ecal-environment.h"

G_BEGIN_DECLS

ECal*          evo_cal_source_open_source                (const char     *uri,
                                                          ECalSourceType  type);
ECal*          evo_cal_source_open_new_with_absolute_uri (const char     *name,
                                                          const char     *uri,
                                                          ECalSourceType  type);
void           evo_cal_source_print_all_objects          (ECal           *ecal);
char*          evo_cal_component_get_uid                 (ECalComponent  *obj);
glong          evo_cal_component_get_modified            (ECalComponent  *obj);
void           evo_cal_component_set_modified            (ECalComponent  *obj,
                                                          glong           seconds);
char*          evo_cal_component_get_summary             (ECalComponent  *obj);
void           evo_cal_component_set_summary             (ECalComponent  *obj,
                                                          const char     *summary);
gboolean       evo_cal_source_remove_object              (ECal           *ecal,
                                                          ECalComponent  *obj);
gboolean       evo_cal_source_update_object              (ECal           *ecal,
                                                          ECalComponent  *obj);
char*          evo_cal_source_add_object                 (ECal           *ecal,
                                                          ECalComponent  *obj);
ECalComponent* evo_cal_source_get_object                 (ECal           *ecal,
                                                          const char     *uid,
                                                          const char     *rid);
char *         evo_cal_source_get_uid                    (ECal           *ecal);
char *         evo_cal_component_get_url                 (ECalComponent  *obj);
void           evo_cal_component_set_url                 (ECalComponent  *obj,
                                                          const char     *url);
char *         evo_cal_component_get_description         (ECalComponent  *obj);
void           evo_cal_component_set_description         (ECalComponent  *obj,
                                                          const char     *desc);
int            evo_cal_component_get_priority            (ECalComponent  *comp);
void           evo_cal_component_set_priority            (ECalComponent  *comp,
                                                          int             priority);
icalproperty_status evo_cal_component_get_status         (ECalComponent  *obj);
void           evo_cal_component_set_status              (ECalComponent  *obj,
                                                          icalproperty_status status);


G_END_DECLS

#endif /* EVO_CALENDAR_H */ 
 
 
