/* PyGtkSourceView - Python bindings for GtkSourceView.
   Copyright (C) 2004 - Stéphan Kochen <g-lite@kochen.nl>
   
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <pygobject.h>
#include <pygtk/pygtk.h>
 
void pygtksourceview_register_classes (PyObject *d); 
extern PyMethodDef pygtksourceview_functions[];
 
DL_EXPORT (void)
initgtksourceview (void)
{
    PyObject *m, *d;
    
    m = Py_InitModule ("gtksourceview", pygtksourceview_functions);
    d = PyModule_GetDict (m);
    
    init_pygobject ();
    init_pygtk ();

    pygtksourceview_register_classes (d);
    
}
