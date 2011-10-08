#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <pygobject.h>
#include <pygtk/pygtk.h>

#include <evince-document.h>
#include <pycairo/pycairo.h>

Pycairo_CAPI_t *Pycairo_CAPI;


void pyevince_register_classes (PyObject *d);
void pyevince_add_constants(PyObject *module, const gchar *strip_prefix);

extern PyMethodDef pyevince_functions[];

PyObject *
_wrap_ev_shutdown (void)
{
/*     g_message("ev_shutdown();"); */
    ev_shutdown();
    Py_INCREF(Py_None);
    return Py_None;
}


DL_EXPORT(void)
initevince(void)
{
    PyObject *m, *d;

    /* Init glib threads asap */
    if (!g_thread_supported ())
       g_thread_init (NULL);

    init_pygobject ();

    pyg_enable_threads ();

    Pycairo_IMPORT;

    ev_init ();

    m = Py_InitModule ("evince", pyevince_functions);
    d = PyModule_GetDict (m);

    pyevince_register_classes (d);
    pyevince_add_constants(m, "EV_");

    if (PyErr_Occurred ()) {
        return;
    }

      /* Call ev_shutdown() on an atexit handler (bug #570622) */
    {
        // note: py_atexit_method_def has to be static, since python keeps a pointer to it
        static PyMethodDef py_atexit_method_def = {NULL, (PyCFunction)_wrap_ev_shutdown, METH_NOARGS, NULL};
        PyObject *py_atexit_func = PyCFunction_NewEx(&py_atexit_method_def, NULL, NULL);
        PyObject *atexit = PyImport_ImportModule("atexit");
        PyObject_CallMethod(atexit, "register", "N", py_atexit_func);
        Py_DECREF(atexit);
    }
}
