/* -*- Mode: C; c-basic-offset: 4 -*- */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <pygobject.h>
#include <libmate/mate-desktop-item.h>


void py_matedesktop_register_classes (PyObject *d);
void py_matedesktop_add_constants(PyObject *module, const gchar *strip_prefix);
extern PyMethodDef py_matedesktop_functions[];

DL_EXPORT(void)
init_matedesktop(void)
{
    PyObject *m, *d;
	
    init_pygobject ();
	if (PyImport_ImportModule("matevfs") == NULL) {
        PyErr_SetString(PyExc_ImportError, "could not import matevfs");
        return;
    }
    
    m = Py_InitModule ("_matedesktop", py_matedesktop_functions);
    d = PyModule_GetDict (m);
	
    py_matedesktop_register_classes (d);
	py_matedesktop_add_constants (m, "MATE_DESKTOP_ITEM_");
	
	if (PyErr_Occurred())
        Py_FatalError("could not initialise module _matedesktop");
}
