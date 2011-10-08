/* 
 * Copyright (C) 2004 Tiago Cogumbreiro <cogumbreiro@users.sf.net>
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
 * Authors: Tiago Cogumbreiro <cogumbreiro@users.sf.net>
 */
#include <Python.h>
#include <caja-burn-recorder.h>
#include "nb_track.h"

/* base class */

static void
nb_Track_dealloc(nb_Track* self)
{
	if (self->track.type == CAJA_BURN_RECORDER_TRACK_TYPE_DATA) {
		g_free (self->track.contents.data.filename);
	} else if (self->track.type == CAJA_BURN_RECORDER_TRACK_TYPE_AUDIO) {
		g_free (self->track.contents.audio.filename);
		g_free (self->track.contents.audio.cdtext);
	}
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
nb_Track_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    nb_Track *self;
	
    self = (nb_Track *)type->tp_alloc(type, 0);
    if (self != NULL) {
	    /* make sure it points to an empty string */
	    self->track.contents.audio.filename = NULL;
	    self->track.contents.audio.cdtext = NULL;
    }

    return (PyObject *)self;
}

/* setters */

static int
nb_AudioTrack_setfilename (nb_Track *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the filename attribute");
		return -1;
	}

	if (! PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, 
                        "The filename attribute value must be a string");
		return -1;
	}
	g_free (self->track.contents.audio.filename);
	self->track.contents.audio.filename = g_strdup (PyString_AsString (value));
	return 0;
}

static int
nb_AudioTrack_setcdtext (nb_Track *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the cdtext attribute");
		return -1;
	}

	if (! PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, 
                        "The cdtext attribute value must be a string");
		return -1;
	}
	g_free (self->track.contents.audio.cdtext);
	self->track.contents.audio.cdtext = g_strdup (PyString_AsString (value));
	return 0;
}

static int
nb_DataTrack_setfilename (nb_Track *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the filename attribute");
		return -1;
	}

	if (! PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, 
                        "The filename attribute value must be a string");
		return -1;
	}
	g_free (self->track.contents.data.filename);
	self->track.contents.data.filename = g_strdup (PyString_AsString (value));
	return 0;
}

/* getters */

static PyObject *
nb_AudioTrack_getfilename(nb_Track *self, void *closure)
{
    return Py_BuildValue ("s", self->track.contents.audio.filename);
}

static PyObject *
nb_AudioTrack_getcdtext(nb_Track *self, void *closure)
{
    return Py_BuildValue ("s", self->track.contents.audio.cdtext);
}

static PyObject *
nb_DataTrack_getfilename(nb_Track *self, void *closure)
{
    return Py_BuildValue ("s", self->track.contents.data.filename);
}

/* initializers */
static int
nb_AudioTrack_init(nb_Track *self, PyObject *args, PyObject *kwds)
{
	char *filename=NULL, *cdtext=NULL;

	static char *kwlist[] = {"filename", "cdtext", NULL};

	if (! PyArg_ParseTupleAndKeywords(args, kwds, "|ss", kwlist, 
                                      &filename, &cdtext))
		return -1; 

	if (!filename)
		filename = "";
    
	if (!cdtext)
		cdtext = "";
    	
    self->track.type = CAJA_BURN_RECORDER_TRACK_TYPE_AUDIO;
    self->track.contents.audio.filename = g_strdup (filename);
    self->track.contents.audio.cdtext = g_strdup (cdtext);
	return 0;
}

static int
nb_DataTrack_init(nb_Track *self, PyObject *args, PyObject *kwds)
{
	char *filename=NULL;

	static char *kwlist[] = {"filename", NULL};

	if (! PyArg_ParseTupleAndKeywords(args, kwds, "|s", kwlist,  &filename))
		return -1; 

	if (!filename)
		filename = "";
    
    self->track.type = CAJA_BURN_RECORDER_TRACK_TYPE_DATA;
    self->track.contents.data.filename = g_strdup (filename);
	return 0;
}

static PyTypeObject nb_Track_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "cajaburn.Track",             /*tp_name*/
    sizeof(nb_Track),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0, /*tp_dealloc*/
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
    "Represents a recorder's track.",           /* tp_doc */
};

/* AudioTrack definition */
static PyGetSetDef nb_AudioTrack_getseters[] = {
    {"filename",
     (getter)nb_AudioTrack_getfilename, (setter)nb_AudioTrack_setfilename,
     "The track filename, if it's a WAV file must have .wav extension. "
     "Otherwise it will be considered a RAW file.",
     NULL},
    {"cdtext", 
     (getter)nb_AudioTrack_getcdtext, (setter)nb_AudioTrack_setcdtext,
     "The track's associated cdtext. Currently it's not implemented.",
     NULL},
    {NULL}  /* Sentinel */
};

static PyGetSetDef nb_DataTrack_getseters[] = {
    {"filename",
     (getter)nb_DataTrack_getfilename, (setter)nb_DataTrack_setfilename,
     "The track filename, it should point to a ISO 9660 file.",
     NULL},
    {NULL}  /* Sentinel */
};

static PyTypeObject nb_AudioTrack_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "cajaburn.AudioTrack", /*tp_name*/
    sizeof(nb_Track),          /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)nb_Track_dealloc,                         /*tp_dealloc*/
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
    "Represents a recorder's audio track.", /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    0,                     /* tp_methods */
    0,                     /* tp_members */
    nb_AudioTrack_getseters,   /* tp_getset */
    &nb_Track_Type,            /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)nb_AudioTrack_init, /* tp_init */
    0,                         /* tp_alloc */
    nb_Track_new,                 /* tp_new */
};

static PyTypeObject nb_DataTrack_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "cajaburn.DataTrack", /*tp_name*/
    sizeof(nb_Track),          /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)nb_Track_dealloc,                         /*tp_dealloc*/
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
    "Represents a recorder's data track.", /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    0,                     /* tp_methods */
    0,                     /* tp_members */
    nb_DataTrack_getseters,   /* tp_getset */
    &nb_Track_Type,            /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)nb_DataTrack_init, /* tp_init */
    0,                         /* tp_alloc */
    nb_Track_new,                 /* tp_new */
};

int
nb_AudioTrack_Check (PyObject *object)
{
	return PyObject_TypeCheck (object, &nb_AudioTrack_Type);
}

int
nb_DataTrack_Check (PyObject *object)
{
	return PyObject_TypeCheck (object, &nb_DataTrack_Type);
}

int
nb_track_init (PyObject *module)
{
	if (PyType_Ready(&nb_Track_Type) < 0)
		return -1;
	if (PyType_Ready(&nb_DataTrack_Type) < 0)
		return -1;
	if (PyType_Ready(&nb_AudioTrack_Type) < 0)
		return -1;

	Py_INCREF(&nb_Track_Type);
	PyModule_AddObject(module, "Track", (PyObject *)&nb_Track_Type);
	
	nb_DataTrack_Type.tp_bases = Py_BuildValue ("(O)", &nb_Track_Type);
	Py_INCREF(&nb_DataTrack_Type);
	PyModule_AddObject(module, "DataTrack", (PyObject *)&nb_DataTrack_Type);
	
	nb_AudioTrack_Type.tp_bases = Py_BuildValue ("(O)", &nb_Track_Type);
	Py_INCREF(&nb_AudioTrack_Type);
	PyModule_AddObject(module, "AudioTrack", (PyObject *)&nb_AudioTrack_Type);
    
	return 0;
}
