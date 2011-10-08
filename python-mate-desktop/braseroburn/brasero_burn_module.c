/* 
 * Copyright (C) 2009 Frederic PEters <fpeters@0d.be>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Authors: Frederic Peters <fpeters@0d.be>
 */

#include <Python.h>
#include <pygobject.h>
#include <brasero-tags.h>
#include <brasero-enums.h>
#include <brasero-error.h>
#include <brasero-status.h>
#include <brasero-burn-lib.h>
#include <brasero-track-type.h>
#include <brasero-track.h>
#include <brasero-track-data.h>
#include <brasero-track-data-cfg.h>
#include <brasero-track-disc.h>
#include <brasero-track-stream.h>
#include <brasero-track-stream-cfg.h>
#include <brasero-track-image.h>
#include <brasero-track-image-cfg.h>
#include <brasero-session.h>
#include <brasero-session-span.h>
#include <brasero-session-cfg.h>
#include <brasero-burn.h>
#include <brasero-burn-options.h>
#include <brasero-burn-dialog.h>
#include <brasero-tool-dialog.h>
#include <brasero-sum-dialog.h>
#include <brasero-blank-dialog.h>

#include "brasero-types.h"

void brasero_burn_register_classes (PyObject *d);
void brasero_burn_add_constants(PyObject *module, const gchar *strip_prefix);
extern PyMethodDef brasero_burn_functions[];

DL_EXPORT(void)
initbraseroburn(void)
{
	PyObject *m, *d;
	
	if (!pygobject_init (-1, -1, -1))
            return;
	
	m = Py_InitModule ("braseroburn", brasero_burn_functions);
	d = PyModule_GetDict (m);
	if (PyErr_Occurred())
		return;

	/* init (mainly GraftPoint) brasero specific classes */
	if (py_brasero_types_init (m))
		return;

	/* init auto-gened classes */
	brasero_burn_register_classes (d);
	brasero_burn_add_constants (m, "BRASERO_BURN_");
}

