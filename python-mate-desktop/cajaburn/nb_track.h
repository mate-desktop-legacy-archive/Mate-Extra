#include <Python.h>

#include <caja-burn-recorder.h>

typedef struct {
    PyObject_HEAD
    
    CajaBurnRecorderTrack track;
} nb_Track;

int nb_track_init (PyObject *module);
int nb_AudioTrack_Check (PyObject *object);
int nb_DataTrack_Check (PyObject *object);
