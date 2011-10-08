/* -*- Mode: C; c-basic-offset: 4 -*- */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <pygobject.h>
#include <libmateprint/mate-print.h>

void pyprint_register_classes (PyObject *d);
extern PyMethodDef pyprint_functions[];

static PyObject *pymateprint_exception;
static PyObject *pymateprint_bad_value_exception;
static PyObject *pymateprint_no_current_point_exception;
static PyObject *pymateprint_no_current_path_exception;
static PyObject *pymateprint_text_corrupt_exception;
static PyObject *pymateprint_bad_context_exception;
static PyObject *pymateprint_no_page_exception;
static PyObject *pymateprint_no_match_exception;
static PyObject *pymateprint_unknown_exception;

gboolean
pymateprint_check_error (MatePrintReturnCode error)
{
    if (error >= 0)
	return FALSE;
    switch (error)
    {
    case MATE_PRINT_ERROR_BADVALUE:
	PyErr_SetString(pymateprint_bad_value_exception, "Bad value");
	return TRUE;
    case MATE_PRINT_ERROR_NOCURRENTPOINT:
	PyErr_SetString(pymateprint_no_current_point_exception, "No current point");
	return TRUE;
    case MATE_PRINT_ERROR_NOCURRENTPATH:
	PyErr_SetString(pymateprint_no_current_path_exception, "No current path");
	return TRUE;
    case MATE_PRINT_ERROR_TEXTCORRUPT:
	PyErr_SetString(pymateprint_text_corrupt_exception, "Corrupt text");
	return TRUE;
    case MATE_PRINT_ERROR_BADCONTEXT:
	PyErr_SetString(pymateprint_bad_context_exception, "Bad context");
	return TRUE;
    case MATE_PRINT_ERROR_NOPAGE:
	PyErr_SetString(pymateprint_no_page_exception, "No page");
	return TRUE;
    case MATE_PRINT_ERROR_NOMATCH:
	PyErr_SetString(pymateprint_no_match_exception, "No match");
	return TRUE;
    case MATE_PRINT_OK:
	g_assert_not_reached();
    case MATE_PRINT_ERROR_UNKNOWN:
    default:
	PyErr_SetString(pymateprint_unknown_exception, "Unknown errror");
	return TRUE;
    }
}


void
pyprint_add_defined_constants (PyObject *dict)
{
#define add_item(short)\
    PyModule_AddStringConstant (dict, #short, MATE_PRINT_##short)
      /* MatePrintConfig keys */
    add_item(KEY_PAPER_SIZE);
    add_item(KEY_PAPER_WIDTH);
    add_item(KEY_PAPER_HEIGHT);
    add_item(KEY_PAPER_ORIENTATION);
    add_item(KEY_PAPER_ORIENTATION_MATRIX);
    add_item(KEY_PAGE_ORIENTATION);
    add_item(KEY_PAGE_ORIENTATION_MATRIX);
    add_item(KEY_LAYOUT);
    add_item(KEY_LAYOUT_WIDTH);
    add_item(KEY_LAYOUT_HEIGHT);
    add_item(KEY_RESOLUTION);
    add_item(KEY_RESOLUTION_DPI);
    add_item(KEY_RESOLUTION_DPI_X);
    add_item(KEY_RESOLUTION_DPI_Y);
    add_item(KEY_NUM_COPIES);
    add_item(KEY_COLLATE);
    add_item(KEY_PAGE_MARGIN_LEFT);
    add_item(KEY_PAGE_MARGIN_RIGHT);
    add_item(KEY_PAGE_MARGIN_TOP);
    add_item(KEY_PAGE_MARGIN_BOTTOM);
    add_item(KEY_PAPER_MARGIN_LEFT);
    add_item(KEY_PAPER_MARGIN_RIGHT);
    add_item(KEY_PAPER_MARGIN_TOP);
    add_item(KEY_PAPER_MARGIN_BOTTOM);
    add_item(KEY_OUTPUT_FILENAME);
    add_item(KEY_DOCUMENT_NAME);
    add_item(KEY_PREFERED_UNIT);
#undef add_item

#define add_item(short)\
    PyModule_AddIntConstant (dict, #short, MATE_##short)
      /* MateFontWeight values */
    add_item(FONT_LIGHTEST);
    add_item(FONT_EXTRA_LIGHT);
    add_item(FONT_THIN);
    add_item(FONT_LIGHT);
    add_item(FONT_BOOK);
    add_item(FONT_REGULAR);
    add_item(FONT_MEDIUM);
    add_item(FONT_SEMI);
    add_item(FONT_DEMI);
    add_item(FONT_BOLD);
    add_item(FONT_HEAVY);
    add_item(FONT_EXTRABOLD);
    add_item(FONT_BLACK);
    add_item(FONT_EXTRABLACK);
    add_item(FONT_HEAVIEST);
#undef add_item

      /* ArtPathcode */
#define add_item(short)\
    PyModule_AddIntConstant (dict, #short, ART_##short)
    add_item(MOVETO);
    add_item(MOVETO_OPEN);
    add_item(CURVETO);
    add_item(LINETO);
#undef add_item

}

static void initialize_exceptions (PyObject *d)
{
    pymateprint_exception = PyErr_NewException ("mateprint.Error",
						 PyExc_RuntimeError, NULL);
    PyDict_SetItemString(d, "Error", pymateprint_exception);

#define register_exception(c_name, py_name)				\
    pymateprint_##c_name##_exception =					\
	PyErr_NewException ("mateprint." py_name "Error",		\
			    pymateprint_exception, NULL);		\
    PyDict_SetItemString(d, py_name "Error", pymateprint_##c_name##_exception);

    register_exception(bad_value,        "BadValue");
    register_exception(no_current_point, "NoCurrentPoint");
    register_exception(no_current_path,  "NoCurrentPath");
    register_exception(text_corrupt,     "TextCorrupt");
    register_exception(bad_context,      "BadContext");
    register_exception(no_page,          "NoPage");
    register_exception(no_match,         "NoMatch");
    register_exception(unknown,          "Unknown");
#undef register_exception
}


DL_EXPORT(void)
init_print (void)
{
    PyObject *m, *d;
	
    init_pygobject ();
    
    m = Py_InitModule ("_print", pyprint_functions);
    d = PyModule_GetDict (m);
    initialize_exceptions (d);
    pyprint_register_classes (d);
    pyprint_add_defined_constants (m);
}

