/* 
 * Copyright (C) 2004 Tiago Cogumbreiro <cogumbreiro@users.sf.net>
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
 * Authors: Tiago Cogumbreiro <cogumbreiro@users.sf.net>
 */

#include <Python.h>
#include <pygobject.h>
#include "nb_track.h"
#include <caja-burn-recorder.h>
void caja_burn_register_classes (PyObject *d);
void caja_burn_add_constants(PyObject *module, const gchar *strip_prefix);
extern PyMethodDef caja_burn_functions[];



DL_EXPORT(void)
initcajaburn(void)
{
	PyObject *m, *d;
	
	init_pygobject ();
	
	m = Py_InitModule ("cajaburn", caja_burn_functions);
	d = PyModule_GetDict (m);
	
	/* init Track classes */
	if (nb_track_init (m))
		return;
	/* init auto-gened classes */
	caja_burn_register_classes (d);
	caja_burn_add_constants (m, "CAJA_BURN_");
}
