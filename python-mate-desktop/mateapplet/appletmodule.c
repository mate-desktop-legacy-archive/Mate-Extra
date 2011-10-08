/* -*- Mode: C; c-basic-offset: 4 -*- */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <pygobject.h>
#include <mate-panel-applet.h>
#include <matecomponent/matecomponent-main.h>
#include <signal.h>
#include <libmate/mate-program.h>
#include <libmateui/mate-ui-init.h>

void pyapplet_register_classes (PyObject *d);

extern PyMethodDef pyapplet_functions[];

DL_EXPORT(void)
initmateapplet(void)
{
    PyObject *m, *d;

    struct sigaction sa;
    PyObject *av;
    int argc, i;
    char **argv;
	
    init_pygobject();
    PyImport_ImportModule("matecomponent.ui");
    if (PyErr_Occurred())
        return;

    m = Py_InitModule ("mateapplet", pyapplet_functions);
    d = PyModule_GetDict (m);
	
    pyapplet_register_classes (d);
    PyModule_AddIntConstant (m, "NO_BACKGROUND", PANEL_NO_BACKGROUND);
    PyModule_AddIntConstant (m, "COLOR_BACKGROUND", PANEL_COLOR_BACKGROUND);
    PyModule_AddIntConstant (m, "PIXMAP_BACKGROUND", PANEL_PIXMAP_BACKGROUND);

    PyModule_AddIntConstant (m, "ORIENT_UP", MATE_PANEL_APPLET_ORIENT_UP);
    PyModule_AddIntConstant (m, "ORIENT_DOWN", MATE_PANEL_APPLET_ORIENT_DOWN);
    PyModule_AddIntConstant (m, "ORIENT_LEFT", MATE_PANEL_APPLET_ORIENT_LEFT);
    PyModule_AddIntConstant (m, "ORIENT_RIGHT", MATE_PANEL_APPLET_ORIENT_RIGHT);

    PyModule_AddIntConstant (m, "SIZE_XX_SMALL", MATE_Vertigo_PANEL_XX_SMALL);
    PyModule_AddIntConstant (m, "SIZE_X_SMALL", MATE_Vertigo_PANEL_X_SMALL);
    PyModule_AddIntConstant (m, "SIZE_SMALL", MATE_Vertigo_PANEL_SMALL);
    PyModule_AddIntConstant (m, "SIZE_MEDIUM", MATE_Vertigo_PANEL_MEDIUM);
    PyModule_AddIntConstant (m, "SIZE_LARGE", MATE_Vertigo_PANEL_LARGE);
    PyModule_AddIntConstant (m, "SIZE_X_LARGE", MATE_Vertigo_PANEL_X_LARGE);
    PyModule_AddIntConstant (m, "SIZE_XX_LARGE", MATE_Vertigo_PANEL_XX_LARGE);

    PyModule_AddIntConstant (m, "EXPAND_MAJOR", MATE_PANEL_APPLET_EXPAND_MAJOR);
    PyModule_AddIntConstant (m, "EXPAND_MINOR", MATE_PANEL_APPLET_EXPAND_MINOR);
    PyModule_AddIntConstant (m, "HAS_HANDLE", MATE_PANEL_APPLET_HAS_HANDLE);

    /* make sure MateCORBA2-python is ready? */
    /* this code is copy-pasted from matecomponentmodule.c */

    av = PySys_GetObject("argv");
    if (av != NULL) {
	argc = PyList_Size(av);

	argv = g_new(char *, argc);
	for (i = 0; i < argc; i++)
	    argv[i] = g_strdup(PyString_AsString(PyList_GetItem(av, i)));
    } else {
	argc = 0;
	argv = NULL;
    }
    memset(&sa, 0, sizeof(sa));
    sigaction(SIGCHLD, NULL, &sa);

    if (!matecomponent_init(&argc, argv)) {
	if (argv != NULL) {
	    for (i = 0; i < argc; i++)
		g_free(argv[i]);
	    g_free(argv);
	}
	sigaction(SIGCHLD, &sa, NULL);
	PyErr_SetString(PyExc_RuntimeError, "could not initialise MateComponent");
	return;
    }
    sigaction(SIGCHLD, &sa, NULL);

    if (argv != NULL) {
	PySys_SetArgv(argc, argv);
	for (i = 0; i < argc; i++)
	    g_free(argv[i]);
	g_free(argv);
    }

    mate_program_module_register (LIBMATEUI_MODULE);
}
