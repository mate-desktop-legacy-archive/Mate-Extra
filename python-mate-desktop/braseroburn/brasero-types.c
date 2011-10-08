/* 
 * Copyright (C) 2009 Philippe Rouquier <bonfire-app@wanadoo.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Authors: Philippe Rouquier <bonfire-app@wanadoo.fr>
 */
#include <Python.h>
#include <brasero-types.h>
#include <brasero-track-data.h>


static PyObject *
py_brasero_graft_point_create (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyBraseroGraftPoint *self;

    self = (PyBraseroGraftPoint *)type->tp_alloc(type, 0);
    if (self != NULL) {
	    /* make sure it points to an empty string */
	    self->graft = g_new0 (BraseroGraftPt, 1);
    }

    return (PyObject *)self;
}

static int
py_brasero_graft_point_init (PyBraseroGraftPoint *self, PyObject *args, PyObject *kwds)
{
	char *uri = NULL;
	char *path = NULL;

	static char *kwlist[] = {"uri", "path", NULL};

	if (! PyArg_ParseTupleAndKeywords (args, kwds, "|ss", kwlist,  &uri, &path))
		return -1; 

	if (!uri)
		uri = "";
	if (!path)
		path = "";

	self->graft->uri = g_strdup (uri);
	self->graft->path = g_strdup (path);
	return 0;
}

static void
py_brasero_graft_point_dealloc (PyBraseroGraftPoint* self)
{
	brasero_graft_point_free (self->graft);
	self->ob_type->tp_free((PyObject*)self);
}

static int
py_brasero_graft_point_set_uri (PyBraseroGraftPoint *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the uri attribute");
		return -1;
	}

	if (! PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, 
                        "The uri attribute value must be a string");
		return -1;
	}
	g_free (self->graft->uri);
	self->graft->uri = g_strdup (PyString_AsString (value));
	return 0;
}

static PyObject *
py_brasero_graft_point_get_uri (PyBraseroGraftPoint *self, void *closure)
{
    return Py_BuildValue ("s", self->graft->uri);
}

static int
py_brasero_graft_point_set_path (PyBraseroGraftPoint *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the path attribute");
		return -1;
	}

	if (! PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, 
                        "The path attribute value must be a string");
		return -1;
	}
	g_free (self->graft->path);
	self->graft->path = g_strdup (PyString_AsString (value));
	return 0;
}

static PyObject *
py_brasero_graft_point_get_path (PyBraseroGraftPoint *self, void *closure)
{
    return Py_BuildValue ("s", self->graft->path);
}

static PyGetSetDef PyBraseroGraftPoint_getseters[] = {
    {"uri",
     (getter)py_brasero_graft_point_get_uri, (setter)py_brasero_graft_point_set_uri,
     "The file uri, it should point to a file on a (GIO) mounted file system.",
     NULL},
    {"path",
     (getter)py_brasero_graft_point_get_path, (setter)py_brasero_graft_point_set_path,
     "The path of the file, it should point to a path on the disc image to be.",
     NULL},
    {NULL}  /* Sentinel */
};

static PyTypeObject PyBraseroGraftPoint_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "braseroburn.GraftPoint", /*tp_name*/
    sizeof(PyBraseroGraftPoint),          /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)py_brasero_graft_point_dealloc,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT, /*tp_flags*/
    "Represents a graft point for a TrackData.", /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    0,                     /* tp_methods */
    0,                     /* tp_members */
    PyBraseroGraftPoint_getseters,   /* tp_getset */
    0,            /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)py_brasero_graft_point_init, /* tp_init */
    0,                         /* tp_alloc */
    py_brasero_graft_point_create,                 /* tp_new */
};

PyObject *
PyBraseroGraftPoint_New (const BraseroGraftPt *graft)
{
	PyBraseroGraftPoint *self;

	self = PyObject_New (PyBraseroGraftPoint, &PyBraseroGraftPoint_Type);
	self->graft->uri = g_strdup (graft->uri?graft->uri:"");
	self->graft->path = g_strdup (graft->path?graft->path:"");
	return (PyObject *)self;
}

int
py_BraseroGraftPoint_Check (PyObject *object)
{
	return PyObject_TypeCheck (object, &PyBraseroGraftPoint_Type);
}

int
py_brasero_types_init (PyObject *module)
{
	if (PyType_Ready (&PyBraseroGraftPoint_Type) < 0)
		return -1;

	Py_INCREF(&PyBraseroGraftPoint_Type);
	PyModule_AddObject(module, "GraftPoint", (PyObject *)&PyBraseroGraftPoint_Type);
    
	return 0;
}

