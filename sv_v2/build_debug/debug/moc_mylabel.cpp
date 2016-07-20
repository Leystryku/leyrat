/****************************************************************************
** Meta object code from reading C++ file 'mylabel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../untitledl/mylabel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mylabel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_mylabel_t {
    QByteArrayData data[16];
    char stringdata0[125];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mylabel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mylabel_t qt_meta_stringdata_mylabel = {
    {
QT_MOC_LITERAL(0, 0, 7), // "mylabel"
QT_MOC_LITERAL(1, 8, 8), // "released"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 1), // "x"
QT_MOC_LITERAL(4, 20, 1), // "y"
QT_MOC_LITERAL(5, 22, 8), // "is_right"
QT_MOC_LITERAL(6, 31, 7), // "pressed"
QT_MOC_LITERAL(7, 39, 10), // "keypressed"
QT_MOC_LITERAL(8, 50, 10), // "QKeyEvent*"
QT_MOC_LITERAL(9, 61, 5), // "event"
QT_MOC_LITERAL(10, 67, 8), // "NewImage"
QT_MOC_LITERAL(11, 76, 3), // "img"
QT_MOC_LITERAL(12, 80, 4), // "size"
QT_MOC_LITERAL(13, 85, 11), // "slotPressed"
QT_MOC_LITERAL(14, 97, 12), // "slotReleased"
QT_MOC_LITERAL(15, 110, 14) // "slotKeyPressed"

    },
    "mylabel\0released\0\0x\0y\0is_right\0pressed\0"
    "keypressed\0QKeyEvent*\0event\0NewImage\0"
    "img\0size\0slotPressed\0slotReleased\0"
    "slotKeyPressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mylabel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   49,    2, 0x06 /* Public */,
       6,    3,   56,    2, 0x06 /* Public */,
       7,    1,   63,    2, 0x06 /* Public */,
      10,    2,   66,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,   71,    2, 0x0a /* Public */,
      14,    0,   72,    2, 0x0a /* Public */,
      15,    0,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    3,    4,    5,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::VoidStar, QMetaType::UInt,   11,   12,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void mylabel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        mylabel *_t = static_cast<mylabel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->released((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: _t->pressed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 2: _t->keypressed((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 3: _t->NewImage((*reinterpret_cast< void*(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 4: _t->slotPressed(); break;
        case 5: _t->slotReleased(); break;
        case 6: _t->slotKeyPressed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (mylabel::*_t)(int , int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mylabel::released)) {
                *result = 0;
            }
        }
        {
            typedef void (mylabel::*_t)(int , int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mylabel::pressed)) {
                *result = 1;
            }
        }
        {
            typedef void (mylabel::*_t)(QKeyEvent * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mylabel::keypressed)) {
                *result = 2;
            }
        }
        {
            typedef void (mylabel::*_t)(void * , unsigned int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mylabel::NewImage)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject mylabel::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_mylabel.data,
      qt_meta_data_mylabel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *mylabel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mylabel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_mylabel.stringdata0))
        return static_cast<void*>(const_cast< mylabel*>(this));
    return QLabel::qt_metacast(_clname);
}

int mylabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void mylabel::released(int _t1, int _t2, bool _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void mylabel::pressed(int _t1, int _t2, bool _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void mylabel::keypressed(QKeyEvent * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void mylabel::NewImage(void * _t1, unsigned int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
