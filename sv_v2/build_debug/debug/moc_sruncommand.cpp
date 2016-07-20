/****************************************************************************
** Meta object code from reading C++ file 'sruncommand.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../untitledl/sruncommand.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sruncommand.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SRuncommand_t {
    QByteArrayData data[8];
    char stringdata0[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SRuncommand_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SRuncommand_t qt_meta_stringdata_SRuncommand = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SRuncommand"
QT_MOC_LITERAL(1, 12, 10), // "AddConText"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 11), // "const char*"
QT_MOC_LITERAL(4, 36, 4), // "text"
QT_MOC_LITERAL(5, 41, 5), // "bytes"
QT_MOC_LITERAL(6, 47, 21), // "on_pushButton_pressed"
QT_MOC_LITERAL(7, 69, 14) // "AddConTextSlot"

    },
    "SRuncommand\0AddConText\0\0const char*\0"
    "text\0bytes\0on_pushButton_pressed\0"
    "AddConTextSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SRuncommand[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   34,    2, 0x08 /* Private */,
       7,    2,   35,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,

       0        // eod
};

void SRuncommand::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SRuncommand *_t = static_cast<SRuncommand *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->AddConText((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->on_pushButton_pressed(); break;
        case 2: _t->AddConTextSlot((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SRuncommand::*_t)(const char * , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SRuncommand::AddConText)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject SRuncommand::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SRuncommand.data,
      qt_meta_data_SRuncommand,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SRuncommand::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SRuncommand::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SRuncommand.stringdata0))
        return static_cast<void*>(const_cast< SRuncommand*>(this));
    return QDialog::qt_metacast(_clname);
}

int SRuncommand::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void SRuncommand::AddConText(const char * _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
