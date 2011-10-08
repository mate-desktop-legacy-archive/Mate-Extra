/* -*- Mode: C; c-basic-offset: 4 -*- */
#include <Python.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <pygobject.h>

#include <pygtk/pygtk.h>

void pymarco_register_classes(PyObject *d);
void pymarco_add_constants(PyObject *module, const gchar *strip_prefix);
extern PyMethodDef pymarco_functions[];

DL_EXPORT(void)
initmarco (void)
{
	PyObject *m, *d;

	init_pygobject();
	init_pygtk ();

	m = Py_InitModule ("marco", pymarco_functions);
	d = PyModule_GetDict (m);

	pymarco_register_classes (d);
        pymarco_add_constants(m, "META_");
}
