/* 
   Copyright (C) 2004 Tiago Cogumbreiro <cogumbreiro@users.sf.net>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Mate Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.

   Author: Tiago Cogumbreiro <cogumbreiro@users.sf.net>
 */
#define PY_SSIZE_T_CLEAN
#include <pygobject.h>
#include <Python.h>
#include "totem-pl-parser.h"

#if PY_VERSION_HEX < 0x02050000
typedef int Py_ssize_t;
#define PY_SSIZE_T_MAX INT_MAX
#define PY_SSIZE_T_MIN INT_MIN
typedef inquiry lenfunc;
typedef intargfunc ssizeargfunc;
typedef intobjargproc ssizeobjargproc;
#endif


void pyplparser_register_classes(PyObject *d);
void pyplparser_add_constants(PyObject *module, const gchar *strip_prefix);
extern PyMethodDef pyplparser_functions[];

#ifdef TOTEM_TYPE_PL_PARSER_METADATA
static void
_fill_metadata(gpointer key, gpointer value, gpointer user_data)
{
    PyObject *dict = (PyObject *) user_data;
    PyObject *py_value = PyString_FromString((char *) value);
    PyDict_SetItemString(dict, (char *) key, py_value);
    Py_DECREF(py_value);
}

static PyObject *
_totem_metadata_from_gvalue(const GValue *value)
{
    GHashTable *metadata = (GHashTable*) g_value_get_boxed(value);
    PyObject  *py_metadata = PyDict_New();
    g_hash_table_foreach(metadata, _fill_metadata, py_metadata);
    return py_metadata;
}

static int
_totem_metadata_to_gvalue(GValue *gvalue, PyObject *obj)
{
    PyObject *key, *value;
    Py_ssize_t pos = 0;
    GHashTable *metadata;

    if (!(PyDict_Check(obj))) {
        PyErr_SetString(PyExc_TypeError, "metadata must be a dict");
        return -1;
    }

    metadata = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    
    while (PyDict_Next(obj, &pos, &key, &value)) {
        if (!PyString_Check(key)) {
            PyErr_SetString(PyExc_TypeError, "metadata keys must be strings");
            g_hash_table_destroy(metadata);
            return -1;
        }
        if (!PyString_Check(value)) {
            PyErr_SetString(PyExc_TypeError, "metadata values must be strings");
            g_hash_table_destroy(metadata);
            return -1;
        }
        g_hash_table_insert(metadata, PyString_AsString(key), PyString_AsString(value));
    }

    g_value_take_boxed(gvalue, metadata);
    return 0;
}

#endif

DL_EXPORT(void)
initplparser(void)
{
    PyObject *m, *d;

    init_pygobject();
    if (PyImport_ImportModule("matevfs") == NULL) {
        PyErr_SetString(PyExc_ImportError, 
                        "could not import matevfs");
        return;
    }
    m = Py_InitModule("plparser", pyplparser_functions);
    d = PyModule_GetDict(m);
        
    pyplparser_register_classes(d);
    pyplparser_add_constants(m, "TOTEM_PL_");

#ifdef TOTEM_TYPE_PL_PARSER_METADATA
    pyg_register_gtype_custom(TOTEM_TYPE_PL_PARSER_METADATA,
			      _totem_metadata_from_gvalue,
			      _totem_metadata_to_gvalue);
#endif

}
