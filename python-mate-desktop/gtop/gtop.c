/*
   Copyright (C) 2004 Benoît Dejean <TazForEver at dlfp dot org

   LibGTop is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   LibGTop is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with LibGTop; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/


/*
  dep: libgtop >= 2.8, glib >= 2.5

  gcc -I/usr/include/python2.3 $(pkg-config --cflags --libs libgtop-2.0) \
  -shared -fpic -ggdb -O2 -Wall -std=gnu89 -o gtop.so gtop.c
*/



/* pydoc inside :)
 * don't forget to have a look at it : 'pydoc gtop'
 */



#include <Python.h>

#include <glib.h>

#include <glibtop.h>
#include <glibtop/close.h>
#include <glibtop/signal.h>
#include <glibtop/union.h>
#include <glibtop/sysinfo.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


typedef struct
{
	PyObject_HEAD
	/* Type-specific fields go here. */
	PyObject* dict;
	PyObject* repr; /* cached repr */
} StructObject;


static PyMethodDef Struct_methods[];
static PyTypeObject StructType;



/*
 * Struct Internals
 */


static void _struct_build_repr(StructObject *that)
{
	GString *str;
	PyObject *items;
	int i;

	g_assert(that->repr == NULL);

	str = g_string_new("Struct {");

	/* safe, a Struct can't be empty */
	items = PyDict_Items(that->dict);
	(void) PyList_Sort(items);

	for(i = 0; i < PyList_GET_SIZE(items); ++i) {
		PyObject *pair, *key, *value;

		pair = PyList_GET_ITEM(items, i);

		key   = PyObject_Str(PyTuple_GET_ITEM(pair, 0));
		value = PyObject_Str(PyTuple_GET_ITEM(pair, 1));

		g_string_append_printf(str, " .%s = %s,",
				       PyString_AS_STRING(key),
				       PyString_AS_STRING(value));

		Py_DECREF(key);
		Py_DECREF(value);
	}

	Py_DECREF(items);

	str->str[ str->len - 1 ] = ' '; /* replace the trailing ',' by a ' ' */
	g_string_append_c(str, '}');

	that->repr = PyString_FromStringAndSize(str->str, str->len);

	g_string_free(str, TRUE);
}



/* borrows kw */
static PyObject* _struct_new(PyObject *kw)
{
	StructObject* const that = PyObject_New(StructObject, (PyTypeObject*) &StructType);

	g_assert(that != NULL && kw != NULL);

	that->dict = kw;
	that->repr = NULL;

	return (PyObject*)that;
}




/*
 * Struct Methods
 */


static PyObject* struct_items(PyObject *self, PyObject* args)
{
	StructObject * const that = (StructObject*) self;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	return PyDict_Items(that->dict);
}


static PyObject* struct_dict(PyObject *self, PyObject* args)
{
	StructObject * const that = (StructObject*) self;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	return PyDict_Copy(that->dict);
}



/*
 * Struct Operators
 */



/* should not be used, should be private, defensive copy
 * _struct_new is used internally because it borrows the kw argument
 * which save a PyDict_Copy
 */
static int struct_init(PyObject *self, PyObject *args, PyObject *kw)
{
	StructObject * const that = (StructObject*) self;
	that->dict = NULL;
	that->repr = NULL;

	if(args && !PyArg_ParseTuple(args, ""))
		return -1;

	if(kw == NULL || PyDict_Size(kw) == 0) {
		PyErr_SetString(PyExc_ValueError, "Struct is empty");
		return -1;
	}

	/* TODO: check for valid member names ^[a-zA-Z_] */
	that->dict = PyDict_Copy(kw);

	return 0;
}



static void struct_dealloc(PyObject *self)
{
	StructObject * const that = (StructObject*) self;

	Py_XDECREF(that->dict);
	Py_XDECREF(that->repr);

	self->ob_type->tp_free(self);
}



static PyObject* struct_repr(PyObject *self)
{
	StructObject * const that = (StructObject*) self;

	if(that->repr == NULL) {
		_struct_build_repr(that);
	}

	Py_INCREF(that->repr);
	return that->repr;
}



static long struct_hash(PyObject *self)
{
	StructObject * const that = (StructObject*) self;

	if(that->repr == NULL) {
		_struct_build_repr(that);
	}

	return PyObject_Hash(that->repr);
}



static PyObject* struct_getattr(PyObject *self, /* const */ char *attr)
{
	StructObject * const that = (StructObject*) self;

	PyObject *value;

	value = PyDict_GetItemString(that->dict, attr);

	if(value) {
		Py_INCREF(value);
		return value;
	}

	return Py_FindMethod((PyMethodDef*) Struct_methods, self, attr);
}



static int struct_compare(PyObject *rhs, PyObject *lhs)
{
	StructObject * const right = (StructObject*) rhs;
	StructObject * const left = (StructObject*) lhs;

	return PyObject_Compare(right->dict, left->dict);
}




static PyMethodDef Struct_methods[] =
{
	{"items", struct_items, METH_VARARGS, NULL},
	{"dict",  struct_dict,  METH_VARARGS, NULL},
	{NULL, NULL, 0, NULL}
};


PyDoc_STRVAR(struct_doc, "WTF");


static PyTypeObject StructType =
{
	PyObject_HEAD_INIT(NULL)
	0,			/*ob_size*/
	"gtop._Struct",		/*tp_name*/
	sizeof(StructObject),	/*tp_basicsize*/
	0,			/*tp_itemsize*/
	struct_dealloc,		/*tp_dealloc*/
	0,			/*tp_print*/
	struct_getattr,		 /*tp_getattr*/
	0,			/*tp_setattr*/
	struct_compare,		/*tp_compare*/
	struct_repr,		/*tp_repr*/
	0,			/*tp_as_number*/
	0,			/*tp_as_sequence*/
	0,			/*tp_as_mapping*/
	struct_hash,		/*tp_hash */
	0,			/*tp_call*/
	0,			/*tp_str*/
	0,			/*tp_getattro*/
	0,			/*tp_setattro*/
	0,			/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	struct_doc,		/* tp_doc */
	0,			/* tp_traverse */
	0,			/* tp_clear */
	0,			/* tp_richcompare */
	0,			/* tp_weaklistoffset */
	0,			/* tp_iter */
	0,			/* tp_iternext */
	Struct_methods,		/* tp_methods */
	0,			/* tp_members */
	0,			/* tp_getset */
	0,			/* tp_base */
	0,			/* tp_dict */
	0,			/* tp_descr_get */
	0,			/* tp_descr_set */
	0,			/* tp_dictoffset */
	struct_init,		/* tp_init */
	PyType_GenericAlloc,    /* tp_alloc */
	PyType_GenericNew,      /* tp_new */
	_PyObject_Del,		/* tp_free */
};







/**************************************************************************/
/*                             gtop Functions                             */
/**************************************************************************/

/* some helper macros to fill tuple/list with Struct from an array
 * @<X> : PyObject* pointing to newly <X>. Don't forget to PY_DEREF it when you
 *        don't need it anymore.
 * @converter : function returning a PyObject*
 * @array, @len : source data
 * FILL_<X>  -> converter is called with array[i]
 * FILL_<X>2 -> converter is called with &array[i]
 */

#define INIT_TUPLE_WITH(tuple, converter, array, len) G_STMT_START { \
size_t i; \
tuple = PyTuple_New(len); \
for(i = 0; i < (len); ++i) \
    PyTuple_SET_ITEM(tuple, i, (converter)((array)[i])); \
} G_STMT_END

#define INIT_TUPLE_WITH2(tuple, converter, array, len) G_STMT_START { \
size_t i; \
tuple = PyTuple_New(len); \
for(i = 0; i < (len); ++i) \
    PyTuple_SET_ITEM(tuple, i, (converter)(&(array)[i])); \
} G_STMT_END


#define INIT_LIST_WITH(list, converter, array, len) G_STMT_START { \
size_t i; \
list = PyList_New(len); \
for(i = 0; i < (len); ++i) \
    PyList_SET_ITEM(list, i, (converter)((array)[i])); \
} G_STMT_END

#define INIT_LIST_WITH2(list, converter, array, len) G_STMT_START { \
size_t i; \
list = PyList_New(len); \
for(i = 0; i < (len); ++i) \
    PyList_SET_ITEM(list, i, (converter)(&(array)[i])); \
} G_STMT_END




/* ADD_TO_DICT(dict, key, */

#define ADD_ARRAY_TO_DICT(dict, key, converter, array) G_STMT_START { \
PyObject *tuple = PyTuple_New(G_N_ELEMENTS(array)); \
size_t i=0; \
while(i < G_N_ELEMENTS(array)) \
{ \
PyTuple_SET_ITEM(tuple, i, (converter)((array)[i])); \
++i; \
} \
PyDict_SetItemString(dict, key, tuple); \
} G_STMT_END


#define ADD_ARRAY_TO_DICT2(dict, key, converter, array, size) G_STMT_START { \
PyObject *tuple = PyTuple_New(size); \
size_t i=0; \
while(i < size) \
{ \
PyTuple_SET_ITEM(tuple, i, (converter)((array)[i])); \
++i; \
} \
PyDict_SetItemString(dict, key, tuple); \
} G_STMT_END



/***************************/
/* macros to shorten lines */
/***************************/

#define PyL_ULL PyLong_FromUnsignedLongLong
#define PyL_UL  PyLong_FromUnsignedLong
#define PyF_D   PyFloat_FromDouble
#define PyS_S   PyString_FromString
#define PyS_SaS PyString_FromStringAndSize
#define PyI_L   PyInt_FromLong
#define PyB_L   PyBool_FromLong


/*
  Wrapper for PyDict_SetItemString.
  common usage: my_dict_add_and_decref(d, "magic", PyI_L(42));
 */
static void
my_dict_add_and_decref(PyObject *d, const char *key, PyObject *value)
{
	PyDict_SetItemString(d, key, value);
	/* here value->ob_refcnt is 2 */
	Py_DECREF(value);
}



#if 0
static PyObject* guint64_checked_mult(guint64 lhs, guint64 rhs)
{
	guint64 res = lhs * rhs;

	if(G_LIKELY(res > lhs && res > rhs))
		return PyL_ULL(res);

	else /* overflow */
	{
		PyObject *left, *right, *res;

		left  = PyL_ULL(lhs);
		right = PyL_ULL(rhs);

		res = PyNumber_Multiply(left, right);

		Py_DECREF(left);
		Py_DECREF(right);

		return res;
	}
}
#endif


static PyObject* build_128bit_long(guint64 values[2])
{
	PyObject *res, *x, *y;

	res = PyL_ULL(values[0]);	/* res = values[0]	*/

	x = PyI_L(64);			/* x = 64		*/

	y = PyNumber_Lshift(res, x);	/* y = res << x		*/

	Py_DECREF(x);
	Py_DECREF(res);			/* res = y		*/
	res = y;

	x = PyL_ULL(values[1]);		/* x = values[1]	*/

	y = PyNumber_Add(res, x);	/* y = res + x		*/

	Py_DECREF(x);			/* res = y		*/
	Py_DECREF(res);
	res = y;

	return res;
}


PyDoc_STRVAR(doc_gtop_loadavg,
	     "gtop_loadavg() -> Struct\n"
	     "\n"
	     "Provides load average informations.\n"
	     "\n"

	     "@rtype:  Struct\n"
	     "@return: Struct having all the following keys\n"
	     "\n"

	     "- C{loadavg1  : float}\n"
	     "- C{loadavg5 : float}\n"
	     "- C{loadavg15 : float}\n"
	     "\tNumber of jobs running simultaneously averaged over 1, 5 and 15\n"
	     "\tminutes.\n"
	     "\n"

	     "- C{nr_running : long}\n"
	     "\tNumber of tasks currently running.\n"
	     "\n"

	     "- C{nr_tasks : long}\n"
	     "\tTotal number of tasks.\n"
	     "\n"

	     "- C{last_pid : long}\n"
	     "\tLast PID.\n"
	);

static PyObject* gtop_loadavg(PyObject *self, PyObject *args)
{
	glibtop_loadavg buf;
	PyObject *d;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	glibtop_get_loadavg(&buf);

	d = PyDict_New();

	my_dict_add_and_decref(d, "nr_running", PyL_ULL(buf.nr_running));
	my_dict_add_and_decref(d, "nr_tasks",   PyL_ULL(buf.nr_tasks));
	my_dict_add_and_decref(d, "last_pid",   PyL_ULL(buf.last_pid));
	my_dict_add_and_decref(d, "loadavg1",   PyF_D(buf.loadavg[0]));
	my_dict_add_and_decref(d, "loadavg5",   PyF_D(buf.loadavg[1]));
	my_dict_add_and_decref(d, "loadavg15",  PyF_D(buf.loadavg[2]));

	return _struct_new(d);
}




static PyObject* gtop_uptime(PyObject *self, PyObject *args)
{
	glibtop_uptime buf;
	PyObject *d;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	glibtop_get_uptime(&buf);

	d = PyDict_New();

	my_dict_add_and_decref(d, "boot_time", PyL_ULL(buf.boot_time));
	my_dict_add_and_decref(d, "uptime",    PyF_D(buf.uptime));
	my_dict_add_and_decref(d, "idletime",  PyF_D(buf.idletime));

	return _struct_new(d);
}



PyDoc_STRVAR(doc_gtop_fsusage,
	     "fsusage(mount_dir) -> Struct\n"
	     "\n"
	     "Provices filesystem usage information\n"
	     "\n"
	     "The returned Struct has the following members:\n"
	     "\t'total'\tTotal bytes in the filesystem.\n"
	     "\t'free'\tFree bytes available to the superuser.\n"
	     "\t'avail'\tFree bytes available to ordinary users.\n"
	     "\t'files'\tTotal file nodes.\n"
	     "\t'ffree'\tFree file nodes.\n"
	     "\t'block_size'\tBlock size in byte.\n");

static PyObject* gtop_fsusage(PyObject *self, PyObject *args)
{

	glibtop_fsusage buf;
	const char *mount_dir;
	PyObject *d;

	if(!PyArg_ParseTuple(args, "s", &mount_dir))
		return NULL;

	glibtop_get_fsusage(&buf, mount_dir);

	d = PyDict_New();

	/* (x * block_size) may overflow ?
	 * mult should be done with PyLong
	 * but who has a PB harddrive ?
	 */

	my_dict_add_and_decref(d, "total", PyL_ULL(buf.blocks * buf.block_size));
	my_dict_add_and_decref(d, "free",  PyL_ULL(buf.bfree  * buf.block_size));
	my_dict_add_and_decref(d, "avail", PyL_ULL(buf.bavail * buf.block_size));

	my_dict_add_and_decref(d, "files", PyL_ULL(buf.files));
	my_dict_add_and_decref(d, "ffree", PyL_ULL(buf.ffree));

	my_dict_add_and_decref(d, "block_size", PyL_ULL(buf.block_size));

	my_dict_add_and_decref(d, "read",  PyL_ULL(buf.read));
	my_dict_add_and_decref(d, "write", PyL_ULL(buf.write));

	return _struct_new(d);
}



static PyObject* gtop_proc_args(PyObject *self, PyObject *args)
{
	glibtop_proc_args buf;
	unsigned pid;
	int argc;
	char **argv;
	PyObject *t;

	if(!PyArg_ParseTuple(args, "I", &pid))
		return NULL;

	argv = glibtop_get_proc_argv(&buf, pid, -1);
	argc = g_strv_length(argv);

	INIT_LIST_WITH(t, PyS_S, argv, argc);

	g_strfreev(argv);
	return t;
}







static inline PyObject* mountentry_to_Struct(const glibtop_mountentry *e)
{
	PyObject *d;

	d = PyDict_New();

	my_dict_add_and_decref(d, "dev",      PyL_ULL(e->dev));
	my_dict_add_and_decref(d, "devname",  PyS_S  (e->devname));
	my_dict_add_and_decref(d, "mountdir", PyS_S  (e->mountdir));
	my_dict_add_and_decref(d, "type",     PyS_S  (e->type));

	return _struct_new(d);
}


static PyObject* gtop_mountlist(PyObject *self, PyObject *args)
{
	glibtop_mountlist buf;
	int allfs = FALSE;
	glibtop_mountentry *entries;
	PyObject *t;

	if(!PyArg_ParseTuple(args, "|i", &allfs))
		return NULL;

	entries = glibtop_get_mountlist(&buf, allfs);

	INIT_LIST_WITH2(t, mountentry_to_Struct, entries, buf.number);

	g_free(entries);

	return t;
}




static PyObject* gtop_mem(PyObject *self, PyObject *args)
{
	glibtop_mem buf;
	PyObject *d;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	glibtop_get_mem(&buf);

	d = PyDict_New();

	my_dict_add_and_decref(d, "total",  PyL_ULL(buf.total));
	my_dict_add_and_decref(d, "used",   PyL_ULL(buf.used));
	my_dict_add_and_decref(d, "free",   PyL_ULL(buf.free));
	my_dict_add_and_decref(d, "shared", PyL_ULL(buf.shared));
	my_dict_add_and_decref(d, "buffer", PyL_ULL(buf.buffer));
	my_dict_add_and_decref(d, "cached", PyL_ULL(buf.cached));
	my_dict_add_and_decref(d, "user",   PyL_ULL(buf.user));
	my_dict_add_and_decref(d, "locked", PyL_ULL(buf.locked));

	return _struct_new(d);
}




static PyObject* gtop_swap(PyObject *self, PyObject *args)
{
	glibtop_swap buf;
	PyObject *d;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	glibtop_get_swap(&buf);

	d = PyDict_New();

	my_dict_add_and_decref(d, "total",   PyL_ULL(buf.total));
	my_dict_add_and_decref(d, "used",    PyL_ULL(buf.used));
	my_dict_add_and_decref(d, "free",    PyL_ULL(buf.free));
	my_dict_add_and_decref(d, "pagein",  PyL_ULL(buf.pagein));
	my_dict_add_and_decref(d, "pageout", PyL_ULL(buf.pageout));

	return _struct_new(d);
}


static void hash_table_to_dict_cb(gpointer key, gpointer value, gpointer user_data)
{
	PyObject *d = user_data;
	PyObject *string_value;

	string_value = PyString_FromString(value);

	PyDict_SetItemString(d, key, string_value);

	Py_DECREF(string_value);
}


static PyObject* gtop_sysinfo(PyObject *self, PyObject *args)
{
	const glibtop_sysinfo *infos;
	PyObject *cpus;
	size_t i;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	infos = glibtop_get_sysinfo();
	cpus = PyList_New(0);

	for (i = 0; i < GLIBTOP_NCPU; ++i) {

		const glibtop_entry *entry = &infos->cpuinfo[i];
		PyObject *d;

		if (!entry->values)
			break;

		d = PyDict_New();
		g_hash_table_foreach(entry->values, hash_table_to_dict_cb, d);
		PyList_Append(cpus, d);
		Py_DECREF(d);
	}

	return cpus;
}


static PyObject* gtop_netlist(PyObject *self, PyObject *args)
{
	glibtop_netlist buf;
	PyObject *t;
	char **devices;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	devices = glibtop_get_netlist(&buf);

	INIT_LIST_WITH(t, PyS_S, devices, buf.number);

	g_strfreev(devices);

	return t;
}



static char* hwaddress_format_for_display(const glibtop_netload *buf,
					 char *stringHW, size_t size)
{
	const guint8 * const hw = buf->hwaddress;

	g_snprintf(stringHW, size, "%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
		   hw[0], hw[1], hw[2], hw[3],
		   hw[4], hw[5], hw[6], hw[7]);

	return stringHW;
}


static PyObject* gtop_netload(PyObject *self, PyObject *args)
{
	glibtop_netload buf;
	union {
		char ip4[INET_ADDRSTRLEN];
		char ip6[INET6_ADDRSTRLEN];
		char hw [32]; /* 8 * 2 + 7 * 1 ... 32 is enough :) */
	} addr;
	const char *iface;
	PyObject *d;

	if(!PyArg_ParseTuple(args, "s", &iface))
		return NULL;

	glibtop_get_netload(&buf, iface);

	d = PyDict_New();

	my_dict_add_and_decref(d, "if_flags", PyL_ULL(buf.if_flags));

	my_dict_add_and_decref(d, "mtu", PyL_UL(buf.mtu));
	my_dict_add_and_decref(d, "subnet",
			     PyS_S(inet_ntop(AF_INET, &buf.subnet, addr.ip4, sizeof addr.ip4)));
	my_dict_add_and_decref(d, "address",
			     PyS_S(inet_ntop(AF_INET, &buf.address, addr.ip4, sizeof addr.ip4)));


	my_dict_add_and_decref(d, "packets_in",    PyL_ULL(buf.packets_in));
	my_dict_add_and_decref(d, "packets_out",   PyL_ULL(buf.packets_out));
	my_dict_add_and_decref(d, "packets_total", PyL_ULL(buf.packets_total));

	my_dict_add_and_decref(d, "bytes_in",    PyL_ULL(buf.bytes_in));
	my_dict_add_and_decref(d, "bytes_out",   PyL_ULL(buf.bytes_out));
	my_dict_add_and_decref(d, "bytes_total", PyL_ULL(buf.bytes_total));

	my_dict_add_and_decref(d, "errors_in",    PyL_ULL(buf.errors_in));
	my_dict_add_and_decref(d, "errors_out",   PyL_ULL(buf.errors_out));
	my_dict_add_and_decref(d, "errors_total", PyL_ULL(buf.errors_total));
	my_dict_add_and_decref(d, "collisions",   PyL_ULL(buf.collisions));

	my_dict_add_and_decref(d, "prefix6",
			     PyS_S(inet_ntop(AF_INET6, buf.prefix6, addr.ip6, sizeof addr.ip6)));
	my_dict_add_and_decref(d, "address6",
			     PyS_S(inet_ntop(AF_INET6, buf.address6, addr.ip6, sizeof addr.ip6)));
	my_dict_add_and_decref(d, "scope6", PyI_L(buf.scope6));

	my_dict_add_and_decref(d, "hwaddress",
			     PyS_S(hwaddress_format_for_display(&buf, addr.hw, sizeof addr.hw)));

	return _struct_new(d);
}



static PyObject* gtop_proclist(PyObject *self, PyObject *args)
{
	glibtop_proclist buf;
	PyObject *list;
	pid_t *pids;
	guint64 which = GLIBTOP_KERN_PROC_ALL;
	guint64 arg = 0;

	if(!PyArg_ParseTuple(args, "|KK", &which, &arg))
		return NULL;

	pids = glibtop_get_proclist(&buf, which, arg);

	INIT_LIST_WITH(list, PyI_L, pids, buf.number);

	g_free(pids);

	return list;
}



static PyObject* gtop_ppp(PyObject *self, PyObject *args)
{
	glibtop_ppp buf;
	PyObject *d;
	unsigned short dev;

	if(!PyArg_ParseTuple(args, "H", &dev))
		return NULL;

	glibtop_get_ppp(&buf, dev);

	d = PyDict_New();

	my_dict_add_and_decref(d, "state", PyL_ULL(buf.state));
	my_dict_add_and_decref(d, "bytes_in",  PyL_ULL(buf.bytes_in));
	my_dict_add_and_decref(d, "bytes_out", PyL_ULL(buf.bytes_out));

	return _struct_new(d);
}



static inline PyObject * get_smp_cpu(glibtop_cpu *buf, unsigned i)
{
	PyObject *d;

	d = PyDict_New();

	my_dict_add_and_decref(d, "total", PyL_ULL(buf->xcpu_total[i]));
	my_dict_add_and_decref(d, "user",  PyL_ULL(buf->xcpu_user[i]));
	my_dict_add_and_decref(d, "nice",  PyL_ULL(buf->xcpu_nice[i]));
	my_dict_add_and_decref(d, "sys",   PyL_ULL(buf->xcpu_sys[i]));
	my_dict_add_and_decref(d, "idle",  PyL_ULL(buf->xcpu_idle[i]));
	my_dict_add_and_decref(d, "iowait",	PyL_ULL(buf->xcpu_iowait[i]));
	my_dict_add_and_decref(d, "irq",	PyL_ULL(buf->xcpu_irq[i]));
	my_dict_add_and_decref(d, "sofr_irq",	PyL_ULL(buf->xcpu_softirq[i]));

	return _struct_new(d);
}


static PyObject* gtop_cpu(PyObject *self, PyObject *args)
{
	glibtop_cpu buf;
	PyObject *d;
	PyObject *smp;
	unsigned i;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	glibtop_get_cpu(&buf);

	d = PyDict_New();

	my_dict_add_and_decref(d, "total", PyL_ULL(buf.total));
	my_dict_add_and_decref(d, "user",  PyL_ULL(buf.user));
	my_dict_add_and_decref(d, "nice",  PyL_ULL(buf.nice));
	my_dict_add_and_decref(d, "sys",   PyL_ULL(buf.sys));
	my_dict_add_and_decref(d, "idle",  PyL_ULL(buf.idle));
	my_dict_add_and_decref(d, "frequency", PyL_ULL(buf.frequency));
	my_dict_add_and_decref(d, "iowait",	PyL_ULL(buf.iowait));
	my_dict_add_and_decref(d, "irq",	PyL_ULL(buf.irq));
	my_dict_add_and_decref(d, "sofr_irq",	PyL_ULL(buf.softirq));

	smp = PyTuple_New(1 + glibtop_global_server->ncpu);

	for(i = 0; i <= glibtop_global_server->ncpu; ++i)
	{
		PyObject *a;
		a = get_smp_cpu(&buf, i);
		PyTuple_SET_ITEM(smp, i, a);
	}

	my_dict_add_and_decref(d, "cpus", smp);
	Py_INCREF(smp);
	my_dict_add_and_decref(d, "xcpu", smp);
	my_dict_add_and_decref(d, "xcpu_flags",  PyL_ULL(buf.xcpu_flags));

	return _struct_new(d);
}



static PyObject* gtop_proc_state(PyObject *self, PyObject *args)
{
	glibtop_proc_state buf;
	PyObject *d;
	unsigned pid;

	if(!PyArg_ParseTuple(args, "I", &pid))
		return NULL;

	glibtop_get_proc_state(&buf, pid);

	d = PyDict_New();

	my_dict_add_and_decref(d, "cmd", PyS_S(buf.cmd));
	my_dict_add_and_decref(d, "state", PyL_UL(buf.state));
	my_dict_add_and_decref(d, "uid", PyI_L(buf.uid));
	my_dict_add_and_decref(d, "gid", PyI_L(buf.gid));
	my_dict_add_and_decref(d, "ruid", PyI_L(buf.ruid));
	my_dict_add_and_decref(d, "rgid", PyI_L(buf.rgid));
	my_dict_add_and_decref(d, "has_cpu", PyB_L(buf.has_cpu));
	my_dict_add_and_decref(d, "processor", PyI_L(buf.processor));
	my_dict_add_and_decref(d, "last_processor", PyI_L(buf.last_processor));

	return _struct_new(d);
}



static PyObject* gtop_proc_kernel(PyObject *self, PyObject *args)
{
	glibtop_proc_kernel buf;
	PyObject *d;
	unsigned pid;

	if(!PyArg_ParseTuple(args, "I", &pid))
		return NULL;

	glibtop_get_proc_kernel(&buf, pid);

	d = PyDict_New();

	my_dict_add_and_decref(d, "k_flags", PyL_ULL(buf.k_flags));
	my_dict_add_and_decref(d, "min_flt", PyL_ULL(buf.min_flt));
	my_dict_add_and_decref(d, "maj_flt", PyL_ULL(buf.maj_flt));
	my_dict_add_and_decref(d, "cmin_flt", PyL_ULL(buf.cmin_flt));
	my_dict_add_and_decref(d, "cmaj_flt", PyL_ULL(buf.cmaj_flt));
	my_dict_add_and_decref(d, "kstk_esp", PyL_ULL(buf.kstk_esp));
	my_dict_add_and_decref(d, "kstk_eip", PyL_ULL(buf.kstk_eip));
	my_dict_add_and_decref(d, "nwchan", PyL_ULL(buf.nwchan));
	my_dict_add_and_decref(d, "wchan", PyS_S(buf.wchan));

	return _struct_new(d);
}



static inline PyObject* map_entry_to_Struct(const glibtop_map_entry *e)
{
	PyObject *d;

	d = PyDict_New();

	my_dict_add_and_decref(d, "start",	PyL_ULL(e->start));
	my_dict_add_and_decref(d, "end",	PyL_ULL(e->end));
	my_dict_add_and_decref(d, "size",	PyL_ULL(e->size));
	my_dict_add_and_decref(d, "offset",	PyL_ULL(e->offset));
	my_dict_add_and_decref(d, "perm",	PyL_ULL(e->perm));
	my_dict_add_and_decref(d, "inode",	PyL_ULL(e->inode));
	my_dict_add_and_decref(d, "device",	PyL_ULL(e->device));
	my_dict_add_and_decref(d, "filename",	PyS_S(e->filename));


	my_dict_add_and_decref(d, "rss",	   PyL_ULL(e->rss));
	my_dict_add_and_decref(d, "shared_clean",  PyL_ULL(e->shared_clean));
	my_dict_add_and_decref(d, "shared_dirty",  PyL_ULL(e->shared_dirty));
	my_dict_add_and_decref(d, "private_clean", PyL_ULL(e->private_clean));
	my_dict_add_and_decref(d, "private_dirty", PyL_ULL(e->private_dirty));

	return _struct_new(d);
}


static PyObject* gtop_proc_map(PyObject *self, PyObject *args)
{
	glibtop_proc_map buf;
	unsigned pid;
	glibtop_map_entry *entries;
	PyObject *t;

	if(!PyArg_ParseTuple(args, "I", &pid))
		return NULL;

	entries = glibtop_get_proc_map(&buf, pid);

	INIT_LIST_WITH2(t, map_entry_to_Struct, entries, buf.number);

	g_free(entries);

	return t;
}



static PyObject* gtop_proc_mem(PyObject *self, PyObject *args)
{
	glibtop_proc_mem buf;
	PyObject *d;
	unsigned pid;

	if(!PyArg_ParseTuple(args, "I", &pid))
		return NULL;

	glibtop_get_proc_mem(&buf, pid);

	d = PyDict_New();

	my_dict_add_and_decref(d, "size",		PyL_ULL(buf.size));
	my_dict_add_and_decref(d, "vsize",	PyL_ULL(buf.vsize));
	my_dict_add_and_decref(d, "resident",	PyL_ULL(buf.resident));
	my_dict_add_and_decref(d, "share",	PyL_ULL(buf.share));
	my_dict_add_and_decref(d, "rss",		PyL_ULL(buf.rss));
	my_dict_add_and_decref(d, "rss_rlim",	PyL_ULL(buf.rss_rlim));

	return _struct_new(d);
}



static PyObject* gtop_proc_segment(PyObject *self, PyObject *args)
{
	glibtop_proc_segment buf;
	PyObject *d;
	unsigned pid;

	if(!PyArg_ParseTuple(args, "I", &pid))
		return NULL;

	glibtop_get_proc_segment(&buf, pid);

	d = PyDict_New();

	my_dict_add_and_decref(d, "text_rss",	PyL_ULL(buf.text_rss));
	my_dict_add_and_decref(d, "shlib_rss",	PyL_ULL(buf.shlib_rss));
	my_dict_add_and_decref(d, "data_rss",	PyL_ULL(buf.data_rss));
	my_dict_add_and_decref(d, "stack_rss",	PyL_ULL(buf.stack_rss));
	my_dict_add_and_decref(d, "dirty_size",	PyL_ULL(buf.dirty_size));
	my_dict_add_and_decref(d, "start_code",	PyL_ULL(buf.start_code));
	my_dict_add_and_decref(d, "end_code",	PyL_ULL(buf.end_code));
	my_dict_add_and_decref(d, "start_stack",	PyL_ULL(buf.start_stack));

	return _struct_new(d);
}



static PyObject* gtop_proc_signal(PyObject *self, PyObject *args)
{
	glibtop_proc_signal buf;
	PyObject *d;
	unsigned pid;

	if(!PyArg_ParseTuple(args, "I", &pid))
		return NULL;

	glibtop_get_proc_signal(&buf, pid);

	d = PyDict_New();

	my_dict_add_and_decref(d, "signal",    build_128bit_long(buf.signal));
	my_dict_add_and_decref(d, "blocked",   build_128bit_long(buf.blocked));
	my_dict_add_and_decref(d, "sigignore", build_128bit_long(buf.sigignore));
	my_dict_add_and_decref(d, "sigcatch",  build_128bit_long(buf.sigcatch));

	return _struct_new(d);
}



static inline PyObject * get_smp_time(glibtop_proc_time *buf, unsigned i)
{
	PyObject *d;

	d = PyDict_New();

	my_dict_add_and_decref(d, "utime",  PyL_ULL(buf->xcpu_utime[i]));
	my_dict_add_and_decref(d, "stime",  PyL_ULL(buf->xcpu_stime[i]));

	return _struct_new(d);
}


static PyObject* gtop_proc_time(PyObject *self, PyObject *args)
{
	glibtop_proc_time buf;
	PyObject *d, *smp;
	unsigned pid;
	unsigned i;

	if(!PyArg_ParseTuple(args, "I", &pid))
		return NULL;

	glibtop_get_proc_time(&buf, pid);

	d = PyDict_New();

	my_dict_add_and_decref(d, "start_time",		PyL_ULL(buf.start_time));
	my_dict_add_and_decref(d, "stime",		PyL_ULL(buf.stime));
	my_dict_add_and_decref(d, "rtime",		PyL_ULL(buf.rtime));
	my_dict_add_and_decref(d, "utime",		PyL_ULL(buf.utime));
	my_dict_add_and_decref(d, "cstime",		PyL_ULL(buf.cstime));
	my_dict_add_and_decref(d, "cutime",		PyL_ULL(buf.cutime));
	my_dict_add_and_decref(d, "timeout",		PyL_ULL(buf.timeout));
	my_dict_add_and_decref(d, "it_real_value",	PyL_ULL(buf.it_real_value));
	my_dict_add_and_decref(d, "frequency",		PyL_ULL(buf.frequency));

	smp = PyTuple_New(1 + glibtop_global_server->ncpu);

	for(i = 0; i <= glibtop_global_server->ncpu; ++i)
	{
		PyObject *a;
		a = get_smp_time(&buf, i);
		PyTuple_SET_ITEM(smp, i, a);
	}

	my_dict_add_and_decref(d, "cpus", smp);
	Py_INCREF(smp);
	my_dict_add_and_decref(d, "xcpu", smp);

	return _struct_new(d);
}



static PyObject* gtop_proc_uid(PyObject *self, PyObject *args)
{
	glibtop_proc_uid buf;
	PyObject *d;
	unsigned pid;

	if(!PyArg_ParseTuple(args, "I", &pid))
		return NULL;

	glibtop_get_proc_uid(&buf, pid);

	d = PyDict_New();

	my_dict_add_and_decref(d, "uid", PyL_UL(buf.uid));
	my_dict_add_and_decref(d, "euid", PyL_UL(buf.euid));
	my_dict_add_and_decref(d, "gid", PyL_UL(buf.gid));
	my_dict_add_and_decref(d, "egid", PyL_UL(buf.egid));
	my_dict_add_and_decref(d, "suid", PyL_UL(buf.suid));
	my_dict_add_and_decref(d, "sgid", PyL_UL(buf.sgid));
	my_dict_add_and_decref(d, "fsuid", PyL_UL(buf.fsuid));
	my_dict_add_and_decref(d, "fsgid", PyL_UL(buf.fsgid));
	my_dict_add_and_decref(d, "pid", PyL_UL(buf.pid));
	my_dict_add_and_decref(d, "ppid", PyL_UL(buf.ppid));
	my_dict_add_and_decref(d, "pgrp", PyL_UL(buf.pgrp));
	my_dict_add_and_decref(d, "session", PyL_UL(buf.session));
	my_dict_add_and_decref(d, "tty", PyL_UL(buf.tty));
	my_dict_add_and_decref(d, "tpgid", PyL_UL(buf.tpgid));
	my_dict_add_and_decref(d, "priority", PyL_UL(buf.priority));
	my_dict_add_and_decref(d, "nice", PyL_UL(buf.nice));

	ADD_ARRAY_TO_DICT2(d, "groups", PyL_UL, buf.groups, buf.ngroups);

	return _struct_new(d);
}



static inline PyObject*
open_files_entry_to_Struct(const glibtop_open_files_entry *e)
{
	PyObject *d;

	d = PyDict_New();

	my_dict_add_and_decref(d, "fd", PyI_L(e->fd));
	my_dict_add_and_decref(d, "type", PyI_L(e->type));

	switch(e->type)
	{
	case GLIBTOP_FILE_TYPE_INETSOCKET:
		my_dict_add_and_decref(d, "sock_dest_host", PyS_S(e->info.sock.dest_host));
		my_dict_add_and_decref(d, "sock_dest_port", PyI_L(e->info.sock.dest_port));
		break;

	case GLIBTOP_FILE_TYPE_LOCALSOCKET:
		my_dict_add_and_decref(d, "localsock_name", PyS_S(e->info.localsock.name));
		break;

	case GLIBTOP_FILE_TYPE_FILE:
		my_dict_add_and_decref(d, "file_name", PyS_S(e->info.file.name));
		break;
	}

	return _struct_new(d);
}


static PyObject* gtop_proc_open_files(PyObject *self, PyObject *args)
{
	glibtop_proc_open_files buf;
	unsigned pid;
	glibtop_open_files_entry *entries;
	PyObject *t;

	if(!PyArg_ParseTuple(args, "I", &pid))
		return NULL;

	entries = glibtop_get_proc_open_files(&buf, pid);

	INIT_LIST_WITH2(t, open_files_entry_to_Struct, entries, buf.number);

	g_free(entries);

	return t;
}




static PyMethodDef gtop_methods[] =
{
	{"cpu",		gtop_cpu,		METH_VARARGS, NULL},
	{"fsusage",	gtop_fsusage,		METH_VARARGS, doc_gtop_fsusage},
	{"loadavg",	gtop_loadavg,		METH_VARARGS, doc_gtop_loadavg},
	{"mem",		gtop_mem,		METH_VARARGS, NULL},
	{"mountlist",	gtop_mountlist,		METH_VARARGS, NULL},
	{"netlist",	gtop_netlist,		METH_VARARGS, NULL},
	{"netload",	gtop_netload,		METH_VARARGS, NULL},
	{"ppp",		gtop_ppp,		METH_VARARGS, NULL},
	{"proc_args",	gtop_proc_args,		METH_VARARGS, NULL},
	{"proc_kernel",	gtop_proc_kernel,	METH_VARARGS, NULL},
	{"proc_state",	gtop_proc_state,	METH_VARARGS, NULL},
	{"proc_map",	gtop_proc_map,		METH_VARARGS, NULL},
	{"proc_mem",	gtop_proc_mem,		METH_VARARGS, NULL},
	{"proc_open_files", gtop_proc_open_files, METH_VARARGS, NULL},
	{"proc_segment",gtop_proc_segment,	METH_VARARGS, NULL},
	{"proc_signal", gtop_proc_signal,	METH_VARARGS, NULL},
	{"proc_time",   gtop_proc_time,		METH_VARARGS, NULL},
	{"proc_uid",    gtop_proc_uid,		METH_VARARGS, NULL},
	{"proclist",	gtop_proclist,		METH_VARARGS, NULL},
	{"swap",	gtop_swap,		METH_VARARGS, NULL},
	{"sysinfo",	gtop_sysinfo,		METH_VARARGS, NULL},
	{"uptime",	gtop_uptime,		METH_VARARGS, NULL},
	{NULL,		NULL,			0, NULL}
};






static void register_constants(PyObject *module)
{
/* hum, libgto constant are unsigned long, but the biggest is (1UL << 18) so
   Int should be OK */


#define add_int(NAME, VALUE) PyModule_AddIntConstant(module, NAME, (VALUE))
#define add_flag(NAME, VALUE) add_int(NAME, (1UL << VALUE))

	add_flag("NETLOAD_IF_FLAGS_ALLMULTI",    GLIBTOP_IF_FLAGS_ALLMULTI);
	add_flag("NETLOAD_IF_FLAGS_ALTPHYS",     GLIBTOP_IF_FLAGS_ALTPHYS);
	add_flag("NETLOAD_IF_FLAGS_BROADCAST",   GLIBTOP_IF_FLAGS_BROADCAST);
	add_flag("NETLOAD_IF_FLAGS_DEBUG",	 GLIBTOP_IF_FLAGS_DEBUG);
	add_flag("NETLOAD_IF_FLAGS_LINK0",	 GLIBTOP_IF_FLAGS_LINK0);
	add_flag("NETLOAD_IF_FLAGS_LINK1",	 GLIBTOP_IF_FLAGS_LINK1);
	add_flag("NETLOAD_IF_FLAGS_LINK2",	 GLIBTOP_IF_FLAGS_LINK2);
	add_flag("NETLOAD_IF_FLAGS_LOOPBACK",    GLIBTOP_IF_FLAGS_LOOPBACK);
	add_flag("NETLOAD_IF_FLAGS_MULTICAST",   GLIBTOP_IF_FLAGS_MULTICAST);
	add_flag("NETLOAD_IF_FLAGS_NOARP",	 GLIBTOP_IF_FLAGS_NOARP);
	add_flag("NETLOAD_IF_FLAGS_OACTIVE",     GLIBTOP_IF_FLAGS_OACTIVE);
	add_flag("NETLOAD_IF_FLAGS_POINTOPOINT", GLIBTOP_IF_FLAGS_POINTOPOINT);
	add_flag("NETLOAD_IF_FLAGS_PROMISC",     GLIBTOP_IF_FLAGS_PROMISC);
	add_flag("NETLOAD_IF_FLAGS_RUNNING",     GLIBTOP_IF_FLAGS_RUNNING);
	add_flag("NETLOAD_IF_FLAGS_SIMPLEX",     GLIBTOP_IF_FLAGS_SIMPLEX);
	add_flag("NETLOAD_IF_FLAGS_UP",		 GLIBTOP_IF_FLAGS_UP);

	add_int("NETLOAD_SCOPE6_GLOBAL",  GLIBTOP_IF_IN6_SCOPE_GLOBAL);
	add_int("NETLOAD_SCOPE6_HOST",    GLIBTOP_IF_IN6_SCOPE_HOST);
	add_int("NETLOAD_SCOPE6_LINK",    GLIBTOP_IF_IN6_SCOPE_LINK);
	add_int("NETLOAD_SCOPE6_SITE",    GLIBTOP_IF_IN6_SCOPE_SITE);
	add_int("NETLOAD_SCOPE6_UNKNOWN", GLIBTOP_IF_IN6_SCOPE_UNKNOWN);

	add_int("PROCLIST_KERN_PROC_ALL",	GLIBTOP_KERN_PROC_ALL);
	add_int("PROCLIST_KERN_PROC_PID",	GLIBTOP_KERN_PROC_PID);
	add_int("PROCLIST_KERN_PROC_PGRP",	GLIBTOP_KERN_PROC_PGRP);
	add_int("PROCLIST_KERN_PROC_SESSION",	GLIBTOP_KERN_PROC_SESSION);
	add_int("PROCLIST_KERN_PROC_TTY",	GLIBTOP_KERN_PROC_TTY);
	add_int("PROCLIST_KERN_PROC_UID",	GLIBTOP_KERN_PROC_UID);
	add_int("PROCLIST_KERN_PROC_RUID",	GLIBTOP_KERN_PROC_RUID);
	add_int("PROCLIST_KERN_PROC_MASK",	GLIBTOP_KERN_PROC_MASK);
	add_int("PROCLIST_EXCLUDE_IDLE",	GLIBTOP_EXCLUDE_IDLE);
	add_int("PROCLIST_EXCLUDE_SYSTEM",	GLIBTOP_EXCLUDE_SYSTEM);
	add_int("PROCLIST_EXCLUDE_NOTTY",	GLIBTOP_EXCLUDE_NOTTY);

	add_int("PPP_STATE_UNKNOWN",	GLIBTOP_PPP_STATE_UNKNOWN);
	add_int("PPP_STATE_HANGUP",	GLIBTOP_PPP_STATE_HANGUP);
	add_int("PPP_STATE_ONLINE",	GLIBTOP_PPP_STATE_ONLINE);

	add_int("PROCESS_RUNNING",		GLIBTOP_PROCESS_RUNNING);
	add_int("PROCESS_INTERRUPTIBLE",	GLIBTOP_PROCESS_INTERRUPTIBLE);
	add_int("PROCESS_UNINTERRUPTIBLE",	GLIBTOP_PROCESS_UNINTERRUPTIBLE);
	add_int("PROCESS_ZOMBIE",		GLIBTOP_PROCESS_ZOMBIE);
	add_int("PROCESS_STOPPED",		GLIBTOP_PROCESS_STOPPED);
	add_int("PROCESS_SWAPPING",		GLIBTOP_PROCESS_SWAPPING);
	add_int("PROCESS_DEAD",			GLIBTOP_PROCESS_DEAD);

	add_int("MAP_PERM_READ",	GLIBTOP_MAP_PERM_READ);
	add_int("MAP_PERM_WRITE",	GLIBTOP_MAP_PERM_WRITE);
	add_int("MAP_PERM_EXECUTE", GLIBTOP_MAP_PERM_EXECUTE);
	add_int("MAP_PERM_SHARED",	GLIBTOP_MAP_PERM_SHARED);
	add_int("MAP_PERM_PRIVATE", GLIBTOP_MAP_PERM_PRIVATE);

	add_int("FILE_TYPE_FILE",	 GLIBTOP_FILE_TYPE_FILE);
	add_int("FILE_TYPE_PIPE",	 GLIBTOP_FILE_TYPE_PIPE);
	add_int("FILE_TYPE_INETSOCKET",  GLIBTOP_FILE_TYPE_INETSOCKET);
	add_int("FILE_TYPE_LOCALSOCKET", GLIBTOP_FILE_TYPE_LOCALSOCKET);

#undef add_flag
#undef add_int
}


static PyObject*
build_siglist(void)
{
	size_t i, len;
	const glibtop_signame* p;
	PyObject* tuple;

	for(p = glibtop_sys_siglist; p->number; ++p)
		;

	len = p - glibtop_sys_siglist;

	tuple = PyTuple_New(len);

	for(i = 0; i < len; ++i)
	{
		PyObject* d;
		p = &glibtop_sys_siglist[i];

		d = PyDict_New();
		my_dict_add_and_decref(d, "number", PyI_L(p->number));
		my_dict_add_and_decref(d, "name",   PyS_S(p->name));
		my_dict_add_and_decref(d, "label",  PyS_S(p->label));
		PyTuple_SET_ITEM(tuple, i, _struct_new(d));
	}

	return tuple;
}


/*
 * at last !
 */


static void gtop_at_exit(void)
{
	/* glibtop_close is a macro */
	glibtop_close();
}


PyMODINIT_FUNC
initgtop(void)
{
	PyObject* module;

	glibtop_init();

	Py_AtExit(gtop_at_exit);

	module = Py_InitModule("gtop", (PyMethodDef*) gtop_methods);
	PyType_Ready((PyTypeObject*) &StructType);
	PyObject_SetAttrString(module, "_Struct", (void*) &StructType);

	register_constants(module);
	PyModule_AddObject(module, "siglist", build_siglist());
}
