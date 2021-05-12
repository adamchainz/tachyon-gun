#include "Python.h"
#include <stdlib.h>
#include <limits.h>

// Module state
typedef struct {
#if PY_VERSION_HEX >= 0x03070000
    _PyCFunctionFastWithKeywords original_now;
#else
    _PyCFunctionFast original_now;
#endif
    PyCFunction original_utcnow;
    PyCFunction original_clock_gettime;
#if PY_VERSION_HEX >= 0x03070000
    PyCFunction original_clock_gettime_ns;
#endif
    PyCFunction original_gmtime;
    PyCFunction original_localtime;
    PyCFunction original_strftime;
    PyCFunction original_time;
#if PY_VERSION_HEX >= 0x03070000
    PyCFunction original_time_ns;
#endif
} _time_machine_state;

static inline _time_machine_state*
get_time_machine_state(PyObject *module)
{
    void *state = PyModule_GetState(module);
    assert(state != NULL);
    return (_time_machine_state *)state;
}

/* datetime.datetime.now() */

/*
    This one is the only function using 'vectorcall' on Python 3.8.
*/

static PyObject*
#if PY_VERSION_HEX >= 0x03070000
_time_machine_now(PyTypeObject *type, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
#else
_time_machine_now(PyTypeObject *type, PyObject **args, Py_ssize_t nargs, PyObject *kwnames)
#endif
{
    PyObject *result = NULL;

    PyObject *time_machine_module = PyImport_ImportModule("time_machine");
    PyObject *time_machine_now = PyObject_GetAttrString(time_machine_module, "now");

#if PY_VERSION_HEX >= 0x03080000
    result = _PyObject_Vectorcall(time_machine_now, args, nargs, kwnames);
#else
    /*
        Parse the given arguments into tz - copied from
        datetime_datetime_new code generated by argument clinic
    */
    static const char * const _keywords[] = {"tz", NULL};
    static _PyArg_Parser _parser = {"|O:now", _keywords, 0};
    PyObject *tz = Py_None;

#if PY_VERSION_HEX >= 0x03070000
    if (!_PyArg_ParseStackAndKeywords(args, nargs, kwnames, &_parser, &tz)) {
        goto exit;
    }
#else
    if (!_PyArg_ParseStack(args, nargs, kwnames, &_parser, &tz)) {
        goto exit;
    }
#endif

    /* Call with a new tuple */
    PyObject* now_args = PyTuple_Pack(1, tz);
    result = PyObject_CallObject(time_machine_now, now_args);
    Py_DECREF(now_args);

exit:
#endif

    Py_DECREF(time_machine_now);
    Py_DECREF(time_machine_module);

    return result;
}

static PyObject*
#if PY_VERSION_HEX >= 0x03070000
_time_machine_original_now(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
#else
_time_machine_original_now(PyObject *module, PyObject **args, Py_ssize_t nargs, PyObject *kwnames)
#endif
{
    _time_machine_state *state = get_time_machine_state(module);

    PyObject *datetime_module = PyImport_ImportModule("datetime");
    PyObject *datetime_class = PyObject_GetAttrString(datetime_module, "datetime");

    PyObject* result = state->original_now(datetime_class, args, nargs, kwnames);

    Py_DECREF(datetime_class);
    Py_DECREF(datetime_module);

    return result;
}
PyDoc_STRVAR(original_now_doc,
"original_now() -> datetime\n\
\n\
Call datetime.datetime.now() after patching.");

/* datetime.datetime.utcnow() */

static PyObject*
_time_machine_utcnow(PyObject *cls, PyObject *args)
{
    PyObject *time_machine_module = PyImport_ImportModule("time_machine");
    PyObject *time_machine_utcnow = PyObject_GetAttrString(time_machine_module, "utcnow");

    PyObject* result = PyObject_CallObject(time_machine_utcnow, args);

    Py_DECREF(time_machine_utcnow);
    Py_DECREF(time_machine_module);

    return result;
}

static PyObject*
_time_machine_original_utcnow(PyObject *module, PyObject *args)
{
    _time_machine_state *state = get_time_machine_state(module);

    PyObject *datetime_module = PyImport_ImportModule("datetime");
    PyObject *datetime_class = PyObject_GetAttrString(datetime_module, "datetime");

    PyObject* result = state->original_utcnow(datetime_class, args);

    Py_DECREF(datetime_class);
    Py_DECREF(datetime_module);

    return result;
}
PyDoc_STRVAR(original_utcnow_doc,
"original_utcnow() -> datetime\n\
\n\
Call datetime.datetime.utcnow() after patching.");

/* time.clock_gettime() */

static PyObject*
_time_machine_clock_gettime(PyObject *self, PyObject *args)
{
    PyObject *time_machine_module = PyImport_ImportModule("time_machine");
    PyObject *time_machine_clock_gettime = PyObject_GetAttrString(time_machine_module, "clock_gettime");

    PyObject* result = PyObject_CallObject(time_machine_clock_gettime, args);

    Py_DECREF(time_machine_clock_gettime);
    Py_DECREF(time_machine_module);

    return result;
}

static PyObject*
_time_machine_original_clock_gettime(PyObject *module, PyObject *args)
{
    _time_machine_state *state = get_time_machine_state(module);

    PyObject *time_module = PyImport_ImportModule("time");

    PyObject* result = state->original_clock_gettime(time_module, args);

    Py_DECREF(time_module);

    return result;
}
PyDoc_STRVAR(original_clock_gettime_doc,
"original_clock_gettime() -> floating point number\n\
\n\
Call time.clock_gettime() after patching.");

/* time.clock_gettime_ns() */
#if PY_VERSION_HEX >= 0x03070000

static PyObject*
_time_machine_clock_gettime_ns(PyObject *self, PyObject *args)
{
    PyObject *time_machine_module = PyImport_ImportModule("time_machine");
    PyObject *time_machine_clock_gettime_ns = PyObject_GetAttrString(time_machine_module, "clock_gettime_ns");

    PyObject* result = PyObject_CallObject(time_machine_clock_gettime_ns, args);

    Py_DECREF(time_machine_clock_gettime_ns);
    Py_DECREF(time_machine_module);

    return result;
}

static PyObject*
_time_machine_original_clock_gettime_ns(PyObject *module, PyObject *args)
{
    _time_machine_state *state = get_time_machine_state(module);

    PyObject *time_module = PyImport_ImportModule("time");

    PyObject* result = state->original_clock_gettime_ns(time_module, args);

    Py_DECREF(time_module);

    return result;
}
PyDoc_STRVAR(original_clock_gettime_ns_doc,
"original_clock_gettime_ns() -> floating point number\n\
\n\
Call time.clock_gettime_ns() after patching.");

#endif

/* time.gmtime() */

static PyObject*
_time_machine_gmtime(PyObject *self, PyObject *args)
{
    PyObject *time_machine_module = PyImport_ImportModule("time_machine");
    PyObject *time_machine_gmtime = PyObject_GetAttrString(time_machine_module, "gmtime");

    PyObject* result = PyObject_CallObject(time_machine_gmtime, args);

    Py_DECREF(time_machine_gmtime);
    Py_DECREF(time_machine_module);

    return result;
}

static PyObject*
_time_machine_original_gmtime(PyObject *module, PyObject *args)
{
    _time_machine_state *state = get_time_machine_state(module);

    PyObject *time_module = PyImport_ImportModule("time");

    PyObject* result = state->original_gmtime(time_module, args);

    Py_DECREF(time_module);

    return result;
}
PyDoc_STRVAR(original_gmtime_doc,
"original_gmtime() -> floating point number\n\
\n\
Call time.gmtime() after patching.");

/* time.localtime() */

static PyObject*
_time_machine_localtime(PyObject *self, PyObject *args)
{
    PyObject *time_machine_module = PyImport_ImportModule("time_machine");
    PyObject *time_machine_localtime = PyObject_GetAttrString(time_machine_module, "localtime");

    PyObject* result = PyObject_CallObject(time_machine_localtime, args);

    Py_DECREF(time_machine_localtime);
    Py_DECREF(time_machine_module);

    return result;
}

static PyObject*
_time_machine_original_localtime(PyObject *module, PyObject *args)
{
    _time_machine_state *state = get_time_machine_state(module);

    PyObject *time_module = PyImport_ImportModule("time");

    PyObject* result = state->original_localtime(time_module, args);

    Py_DECREF(time_module);

    return result;
}
PyDoc_STRVAR(original_localtime_doc,
"original_localtime() -> floating point number\n\
\n\
Call time.localtime() after patching.");

/* time.strftime() */

static PyObject*
_time_machine_strftime(PyObject *self, PyObject *args)
{
    PyObject *time_machine_module = PyImport_ImportModule("time_machine");
    PyObject *time_machine_strftime = PyObject_GetAttrString(time_machine_module, "strftime");

    PyObject* result = PyObject_CallObject(time_machine_strftime, args);

    Py_DECREF(time_machine_strftime);
    Py_DECREF(time_machine_module);

    return result;
}

static PyObject*
_time_machine_original_strftime(PyObject *module, PyObject *args)
{
    _time_machine_state *state = get_time_machine_state(module);

    PyObject *time_module = PyImport_ImportModule("time");

    PyObject* result = state->original_strftime(time_module, args);

    Py_DECREF(time_module);

    return result;
}
PyDoc_STRVAR(original_strftime_doc,
"original_strftime() -> floating point number\n\
\n\
Call time.strftime() after patching.");

/* time.time() */

static PyObject*
_time_machine_time(PyObject *self, PyObject *args)
{
    PyObject *time_machine_module = PyImport_ImportModule("time_machine");
    PyObject *time_machine_time = PyObject_GetAttrString(time_machine_module, "time");

    PyObject* result = PyObject_CallObject(time_machine_time, args);

    Py_DECREF(time_machine_time);
    Py_DECREF(time_machine_module);

    return result;
}

static PyObject*
_time_machine_original_time(PyObject *module, PyObject *args)
{
    _time_machine_state *state = get_time_machine_state(module);

    PyObject *time_module = PyImport_ImportModule("time");

    PyObject* result = state->original_time(time_module, args);

    Py_DECREF(time_module);

    return result;
}
PyDoc_STRVAR(original_time_doc,
"original_time() -> floating point number\n\
\n\
Call time.time() after patching.");

/* time.time_ns() */
#if PY_VERSION_HEX >= 0x03070000

static PyObject*
_time_machine_time_ns(PyObject *self, PyObject *args)
{
    PyObject *time_machine_module = PyImport_ImportModule("time_machine");
    PyObject *time_machine_time_ns = PyObject_GetAttrString(time_machine_module, "time_ns");

    PyObject* result = PyObject_CallObject(time_machine_time_ns, args);

    Py_DECREF(time_machine_time_ns);
    Py_DECREF(time_machine_module);

    return result;
}

static PyObject*
_time_machine_original_time_ns(PyObject *module, PyObject *args)
{
    _time_machine_state *state = get_time_machine_state(module);

    PyObject *time_module = PyImport_ImportModule("time");

    PyObject* result = state->original_time_ns(time_module, args);

    Py_DECREF(time_module);

    return result;
}
PyDoc_STRVAR(original_time_ns_doc,
"original_time_ns() -> floating point number\n\
\n\
Call time.time_ns() after patching.");

#endif

static PyObject*
_time_machine_patch_if_needed(PyObject *module, PyObject *unused)
{
    _time_machine_state *state = PyModule_GetState(module);
    if (state == NULL) {
        return NULL;
    }

    if (state->original_time)
        Py_RETURN_NONE;

    PyObject *datetime_module = PyImport_ImportModule("datetime");
    PyObject *datetime_class = PyObject_GetAttrString(datetime_module, "datetime");

    PyCFunctionObject *datetime_datetime_now = (PyCFunctionObject *) PyObject_GetAttrString(datetime_class, "now");
#if PY_VERSION_HEX >= 0x03070000
    state->original_now = (_PyCFunctionFastWithKeywords) datetime_datetime_now->m_ml->ml_meth;
#else
    state->original_now = (_PyCFunctionFast) datetime_datetime_now->m_ml->ml_meth;
#endif
    datetime_datetime_now->m_ml->ml_meth = (PyCFunction) _time_machine_now;
    Py_DECREF(datetime_datetime_now);

    PyCFunctionObject *datetime_datetime_utcnow = (PyCFunctionObject *) PyObject_GetAttrString(datetime_class, "utcnow");
    state->original_utcnow = datetime_datetime_utcnow->m_ml->ml_meth;
    datetime_datetime_utcnow->m_ml->ml_meth = _time_machine_utcnow;
    Py_DECREF(datetime_datetime_utcnow);

    Py_DECREF(datetime_class);
    Py_DECREF(datetime_module);

    PyObject *time_module = PyImport_ImportModule("time");



    PyCFunctionObject *time_clock_gettime = (PyCFunctionObject *) PyObject_GetAttrString(time_module, "clock_gettime");
    /*
        time.clock_gettime() is not always available
        e.g. on builds against old macOS = official Python.org installer
    */
    if (time_clock_gettime != NULL) {
        state->original_clock_gettime = time_clock_gettime->m_ml->ml_meth;
        time_clock_gettime->m_ml->ml_meth = _time_machine_clock_gettime;
        Py_DECREF(time_clock_gettime);
    }

#if PY_VERSION_HEX >= 0x03070000
    PyCFunctionObject *time_clock_gettime_ns = (PyCFunctionObject *) PyObject_GetAttrString(time_module, "clock_gettime_ns");
    if (time_clock_gettime_ns != NULL) {
        state->original_clock_gettime_ns = time_clock_gettime_ns->m_ml->ml_meth;
        time_clock_gettime_ns->m_ml->ml_meth = _time_machine_clock_gettime_ns;
        Py_DECREF(time_clock_gettime_ns);
    }
#endif

    PyCFunctionObject *time_gmtime = (PyCFunctionObject *) PyObject_GetAttrString(time_module, "gmtime");
    state->original_gmtime = time_gmtime->m_ml->ml_meth;
    time_gmtime->m_ml->ml_meth = _time_machine_gmtime;
    Py_DECREF(time_gmtime);

    PyCFunctionObject *time_localtime = (PyCFunctionObject *) PyObject_GetAttrString(time_module, "localtime");
    state->original_localtime = time_localtime->m_ml->ml_meth;
    time_localtime->m_ml->ml_meth = _time_machine_localtime;
    Py_DECREF(time_localtime);

    PyCFunctionObject *time_strftime = (PyCFunctionObject *) PyObject_GetAttrString(time_module, "strftime");
    state->original_strftime = time_strftime->m_ml->ml_meth;
    time_strftime->m_ml->ml_meth = _time_machine_strftime;
    Py_DECREF(time_strftime);

    PyCFunctionObject *time_time = (PyCFunctionObject *) PyObject_GetAttrString(time_module, "time");
    state->original_time = time_time->m_ml->ml_meth;
    time_time->m_ml->ml_meth = _time_machine_time;
    Py_DECREF(time_time);

#if PY_VERSION_HEX >= 0x03070000
    PyCFunctionObject *time_time_ns = (PyCFunctionObject *) PyObject_GetAttrString(time_module, "time_ns");
    state->original_time_ns = time_time_ns->m_ml->ml_meth;
    time_time_ns->m_ml->ml_meth = _time_machine_time_ns;
    Py_DECREF(time_time_ns);
#endif

    Py_DECREF(time_module);

    Py_RETURN_NONE;
}
PyDoc_STRVAR(patch_if_needed_doc,
"patch_if_needed() -> None\n\
\n\
Swap in helpers.");



PyDoc_STRVAR(module_doc, "_time_machine module");

static PyMethodDef module_functions[] = {
#if PY_VERSION_HEX >= 0x03070000
    {"original_now", (PyCFunction)_time_machine_original_now, METH_FASTCALL|METH_KEYWORDS, original_now_doc},
#else
    {"original_now", (PyCFunction)_time_machine_original_now, METH_FASTCALL, original_now_doc},
#endif
    {"original_utcnow", (PyCFunction)_time_machine_original_utcnow, METH_NOARGS, original_utcnow_doc},
    {"original_clock_gettime", (PyCFunction)_time_machine_original_clock_gettime, METH_VARARGS, original_clock_gettime_doc},
#if PY_VERSION_HEX >= 0x03070000
    {"original_clock_gettime_ns", (PyCFunction)_time_machine_original_clock_gettime_ns, METH_VARARGS, original_clock_gettime_ns_doc},
#endif
    {"original_gmtime", (PyCFunction)_time_machine_original_gmtime, METH_VARARGS, original_gmtime_doc},
    {"original_localtime", (PyCFunction)_time_machine_original_localtime, METH_VARARGS, original_localtime_doc},
    {"original_strftime", (PyCFunction)_time_machine_original_strftime, METH_VARARGS, original_strftime_doc},
    {"original_time", (PyCFunction)_time_machine_original_time, METH_NOARGS, original_time_doc},
#if PY_VERSION_HEX >= 0x03070000
    {"original_time_ns", (PyCFunction)_time_machine_original_time_ns, METH_NOARGS, original_time_ns_doc},
#endif
    {"patch_if_needed", (PyCFunction)_time_machine_patch_if_needed, METH_NOARGS, patch_if_needed_doc},
    {NULL, NULL}  /* sentinel */
};

static PyModuleDef_Slot _time_machine_slots[] = {
    {0, NULL}
};

static struct PyModuleDef _time_machine_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "_time_machine",
    .m_doc = module_doc,
    .m_size = sizeof(_time_machine_state),
    .m_methods = module_functions,
    .m_slots = _time_machine_slots,
    .m_traverse = NULL,
    .m_clear = NULL,
    .m_free = NULL
};

PyMODINIT_FUNC
PyInit__time_machine(void)
{
    return PyModuleDef_Init(&_time_machine_module);
}
