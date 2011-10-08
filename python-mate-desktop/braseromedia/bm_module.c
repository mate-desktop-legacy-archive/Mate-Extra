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
#include <brasero-media.h>

void brasero_media_register_classes (PyObject *d);
void brasero_media_add_constants(PyObject *module, const gchar *strip_prefix);
extern PyMethodDef brasero_media_functions[];

DL_EXPORT(void)
initbraseromedia(void)
{
	PyObject *m, *d;
	
	init_pygobject ();
	pyg_enable_threads ();
	
	m = Py_InitModule ("braseromedia", brasero_media_functions);
	d = PyModule_GetDict (m);
	if (PyErr_Occurred())
		return;
	
	/* init auto-gened classes */
	brasero_media_register_classes (d);
	brasero_media_add_constants (m, "BRASERO_MEDIA_");
}
