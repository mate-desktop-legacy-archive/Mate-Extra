/* -*- Mode: C; c-basic-offset: 4 -*- */
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <pygobject.h>
#include <pygtk/pygtk.h>

#include <profiles/mate-media-profiles.h>

void pymediaprofiles_register_classes(PyObject *d);
extern PyMethodDef pymediaprofiles_functions[];

DL_EXPORT (void)
initmediaprofiles (void)
{
    PyObject *m, *d;
    
    mate_media_profiles_init (mateconf_client_get_default ());

    m = Py_InitModule("mediaprofiles", pymediaprofiles_functions);
    d = PyModule_GetDict(m);
    if (PyErr_Occurred())
        return;

    init_pygtk();
    if (PyErr_Occurred())
        return;
    init_pygobject();
    if (PyErr_Occurred())
        return;
    
    pymediaprofiles_register_classes(d);
    if (PyErr_Occurred())
        return;
}
