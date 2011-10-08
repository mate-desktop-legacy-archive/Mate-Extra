/* -*- Mode: C; c-basic-offset: 4 -*- */
#include <Python.h>
#include <mate-keyring.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <pygobject.h>

static PyObject *PyGKExc_Error;

static PyObject *PyGKExc_DeniedError; /* MATE_KEYRING_RESULT_DENIED */
static PyObject *PyGKExc_NoKeyringDaemonError; /* MATE_KEYRING_RESULT_NO_KEYRING_DAEMON */
static PyObject *PyGKExc_AlreadyUnlockedError; /* MATE_KEYRING_RESULT_ALREADY_UNLOCKED */
static PyObject *PyGKExc_NoSuchKeyringError; /* MATE_KEYRING_RESULT_NO_SUCH_KEYRING */
static PyObject *PyGKExc_BadArgumentsError; /* MATE_KEYRING_RESULT_BAD_ARGUMENTS */
static PyObject *PyGKExc_IOError; /* MATE_KEYRING_RESULT_IO_ERROR */
static PyObject *PyGKExc_CancelledError; /* MATE_KEYRING_RESULT_CANCELLED */
static PyObject *PyGKExc_AlreadyExistsError; /* MATE_KEYRING_RESULT_ALREADY_EXISTS */
#if HAVE_KEYRING_2_20_1
static PyObject *PyGKExc_NoMatchError; /* MATE_KEYRING_RESULT_NO_MATCH */
#endif

PyObject *
pymatekeyring_result_to_exception(MateKeyringResult result)
{
    switch(result)
    {
#define keyring_result_case(result_suffix, exc_middle)  \
    case MATE_KEYRING_RESULT_##result_suffix:          \
        return PyGKExc_##exc_middle##Error;

        keyring_result_case(DENIED, Denied);
        keyring_result_case(NO_KEYRING_DAEMON, NoKeyringDaemon);
        keyring_result_case(ALREADY_UNLOCKED, AlreadyUnlocked);
        keyring_result_case(NO_SUCH_KEYRING, NoSuchKeyring);
        keyring_result_case(BAD_ARGUMENTS, BadArguments);
        keyring_result_case(IO_ERROR, IO);
        keyring_result_case(CANCELLED, Cancelled);
        keyring_result_case(ALREADY_EXISTS, AlreadyExists);
#if HAVE_KEYRING_2_20_1
        keyring_result_case(NO_MATCH, NoMatch);
#endif

#undef keyring_result_case

    case MATE_KEYRING_RESULT_OK:
        return Py_None;

    default:
        g_warning("unhandled MateKeyringResult code %i => "
                  "http://bugzilla.mate.org/enter_bug.cgi?product=mate-python-desktop",
                  result);
        return PyGKExc_Error;
    }
}

gboolean
pymatekeyring_result_check(MateKeyringResult result)
{
    PyObject *exc;

    /* be optimistic */
    if (G_LIKELY(result == MATE_KEYRING_RESULT_OK))
	return FALSE;

    exc = pymatekeyring_result_to_exception(result);

    PyErr_SetNone(exc);
    return TRUE;
}

static void
initialize_exceptions(PyObject *d)
{
    PyGKExc_Error = PyErr_NewException("matekeyring.Error",
                                       PyExc_Exception, NULL);
    PyDict_SetItemString(d, "Error", PyGKExc_Error);

#define register_exception(name)                                        \
    PyGKExc_##name##Error =                                             \
        PyErr_NewException("matekeyring." #name "Error",               \
                            PyGKExc_Error, NULL);                       \
    PyDict_SetItemString(d, #name "Error", PyGKExc_##name##Error);


    register_exception(Denied);
    register_exception(NoKeyringDaemon);
    register_exception(AlreadyUnlocked);
    register_exception(NoSuchKeyring);
    register_exception(BadArguments);
    register_exception(IO);
    register_exception(Cancelled);
    register_exception(AlreadyExists);
#if HAVE_KEYRING_2_20_1
    register_exception(NoMatch);
#endif

#undef register_exception
}


void pymatekeyring_register_classes(PyObject *d);
void pymatekeyring_add_constants(PyObject *module, const gchar *strip_prefix);
extern PyMethodDef pymatekeyring_functions[];

DL_EXPORT(void)
initmatekeyring(void)
{
    PyObject *m, *d;

    init_pygobject();

    m = Py_InitModule("matekeyring", pymatekeyring_functions);
    d = PyModule_GetDict(m);
    initialize_exceptions(d);
    pymatekeyring_register_classes(d);
    pymatekeyring_add_constants(m, "MATE_KEYRING_");
}

