/* -*- Mode: C; c-basic-offset: 4 -*- */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <pygobject.h>
#include <pygtk/pygtk.h>

#include <libmateprintui/mate-print-dialog.h>
#include <libmateprintui/mate-font-dialog.h>
#include <libmateprintui/mate-print-job-preview.h>
#include <libmateprintui/mate-print-preview.h>
#include <libmateprintui/mate-print-paper-selector.h>
#include <libmateprintui/mate-print-unit-selector.h>

void pyprintui_register_classes (PyObject *d);
extern PyMethodDef pyprintui_functions[];

void
pyprintui_add_defined_constants (PyObject *dict)
{
#define add_item(short)\
    PyDict_SetItemString (dict, #short, PyInt_FromLong (MATE_PRINT_##short))
      /* MatePrintRangeType */
    add_item(RANGETYPE_NONE);
    add_item(RANGETYPE_CUSTOM);
    add_item(RANGETYPE_PAGES);
      /* MatePrintDialogRangeFlags */
    add_item(RANGE_CURRENT);
    add_item(RANGE_ALL);
    add_item(RANGE_RANGE);
    add_item(RANGE_SELECTION);
    add_item(RANGE_SELECTION_UNSENSITIVE);
      /* MatePrintDialogFlags */
    add_item(DIALOG_RANGE);
    add_item(DIALOG_COPIES);
      /* MatePrintButtons */
    add_item(DIALOG_RESPONSE_PRINT);
    add_item(DIALOG_RESPONSE_PREVIEW);
    add_item(DIALOG_RESPONSE_CANCEL);
#undef add_item

#define add_item(short)\
    PyDict_SetItemString (dict, #short, PyInt_FromLong (MATE_##short))
    add_item(PAPER_SELECTOR_MARGINS);
    add_item(PAPER_SELECTOR_FEED_ORIENTATION);
#undef add_item
}


DL_EXPORT(void)
initui (void)
{
    PyObject *m, *d;
	
    init_pygobject ();
    init_pygtk();
    
    m = Py_InitModule ("ui", pyprintui_functions);
    d = PyModule_GetDict (m);
	
    pyprintui_register_classes (d);
    pyprintui_add_defined_constants (d);
}

