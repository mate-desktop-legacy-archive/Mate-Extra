/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <pygobject.h>
#include <pycairo.h>
#include <librsvg/rsvg.h>

Pycairo_CAPI_t *Pycairo_CAPI;

void pyrsvg_register_classes(PyObject *d);

extern const PyMethodDef pyrsvg_functions[];


DL_EXPORT(void)
initrsvg(void)
{
    PyObject *rsvg_module = NULL;
    PyObject *dict = NULL;

    init_pygobject();
    rsvg_init();

    Pycairo_IMPORT;

    rsvg_module = Py_InitModule3("rsvg", (PyMethodDef*) pyrsvg_functions,
                                 "rsvg module.");

    dict = PyModule_GetDict(rsvg_module);
    pyrsvg_register_classes(dict);
    if (PyErr_Occurred()) {
        PyErr_Print();
        Py_FatalError("can't initialise module rsvg");
    }
}
