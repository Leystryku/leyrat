/****************************************************************************
** Meta object code from reading C++ file 'filebrowser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../untitledl/filebrowser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filebrowser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Filebrowser_t {
    QByteArrayData data[11];
    char stringdata0[213];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Filebrowser_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Filebrowser_t qt_meta_stringdata_Filebrowser = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Filebrowser"
QT_MOC_LITERAL(1, 12, 23), // "on_pushButton_5_pressed"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 21), // "on_pushButton_pressed"
QT_MOC_LITERAL(4, 59, 23), // "on_pushButton_2_pressed"
QT_MOC_LITERAL(5, 83, 23), // "on_pushButton_4_pressed"
QT_MOC_LITERAL(6, 107, 27), // "on_treeWidget_doubleClicked"
QT_MOC_LITERAL(7, 135, 5), // "index"
QT_MOC_LITERAL(8, 141, 23), // "on_pushButton_3_pressed"
QT_MOC_LITERAL(9, 165, 23), // "on_pushButton_6_pressed"
QT_MOC_LITERAL(10, 189, 23) // "on_pushButton_7_pressed"

    },
    "Filebrowser\0on_pushButton_5_pressed\0"
    "\0on_pushButton_pressed\0on_pushButton_2_pressed\0"
    "on_pushButton_4_pressed\0"
    "on_treeWidget_doubleClicked\0index\0"
    "on_pushButton_3_pressed\0on_pushButton_6_pressed\0"
    "on_pushButton_7_pressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Filebrowser[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    1,   58,    2, 0x08 /* Private */,
       8,    0,   61,    2, 0x08 /* Private */,
       9,    0,   62,    2, 0x08 /* Private */,
      10,    0,   63,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Filebrowser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Filebrowser *_t = static_cast<Filebrowser *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_5_pressed(); break;
        case 1: _t->on_pushButton_pressed(); break;
        case 2: _t->on_pushButton_2_pressed(); break;
        case 3: _t->on_pushButton_4_pressed(); break;
        case 4: _t->on_treeWidget_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 5: _t->on_pushButton_3_pressed(); break;
        case 6: _t->on_pushButton_6_pressed(); break;
        case 7: _t->on_pushButton_7_pressed(); break;
        default: ;
        }
    }
}

const QMetaObject Filebrowser::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Filebrowser.data,
      qt_meta_data_Filebrowser,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Filebrowser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Filebrowser::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Filebrowser.stringdata0))
        return static_cast<void*>(const_cast< Filebrowser*>(this));
    return QDialog::qt_metacast(_clname);
}

int Filebrowser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
