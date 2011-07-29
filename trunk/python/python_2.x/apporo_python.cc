#include <Python.h>
#include "apporo.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace apporo;

static PyObject *apporo_new(PyObject *self, PyObject *args) {
  const char* file_path_;
  if (!PyArg_ParseTuple(args, "s", &file_path_)) {
    return NULL;
  }
  string file_path(file_path_);
  Apporo *app = new Apporo(file_path);
  return (PyObject *)app;
}

static PyObject *apporo_approximate_match(PyObject *self, PyObject *args) {
  PyObject *res = NULL;
  Apporo *app;
  const char* query_;
  if (!PyArg_ParseTuple(args, "Os", &app, &query_)) {
    return NULL;
  }
  string query(query_);
  vector <string> list = app->approximateMatch(query);
  
  if (!list.empty()) {
    int list_len = list.size();
    res = PyTuple_New(list_len);
    for (int i = 0; i < list_len; i++) {
      const char* str = list[i].c_str();
      PyTuple_SetItem(res, i, Py_BuildValue("s", str));
    }
  }
  return (PyObject *)res;
}

static PyObject *apporo_delete(PyObject *self, PyObject *args) {
  Apporo *app;
  if (!PyArg_ParseTuple(args, "O", &app)) {
    return NULL;
  }
  delete app;
  Py_RETURN_TRUE;
}

static PyMethodDef MethodDef[] = {
  {"new", apporo_new, METH_VARARGS, NULL},
  {"approximate_match", apporo_approximate_match, METH_VARARGS, NULL},
  {"delete", apporo_delete, METH_VARARGS, NULL},
  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initapporo(void) {
  PyObject *p;
  p = Py_InitModule("apporo", MethodDef);
}
