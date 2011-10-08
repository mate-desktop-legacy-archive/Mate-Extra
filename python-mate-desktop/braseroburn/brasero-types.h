#include <Python.h>

#include <brasero-track-data.h>

struct _PyBraseroGraftPoint {
    PyObject_HEAD
    
    BraseroGraftPt *graft;
};

typedef struct _PyBraseroGraftPoint PyBraseroGraftPoint;

int py_brasero_types_init (PyObject *module);
int py_BraseroGraftPoint_Check (PyObject *object);

PyObject *PyBraseroGraftPoint_New (const BraseroGraftPt *graft);
