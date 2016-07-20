/****************************************************************************
** Meta object code from reading C++ file 'screenshare.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../untitledl/screenshare.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'screenshare.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Screenshare_t {
    QByteArrayData data[22];
    char stringdata0[283];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Screenshare_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Screenshare_t qt_meta_stringdata_Screenshare = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Screenshare"
QT_MOC_LITERAL(1, 12, 8), // "NewImage"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 3), // "img"
QT_MOC_LITERAL(4, 26, 12), // "slotNewImage"
QT_MOC_LITERAL(5, 39, 16), // "on_label_pressed"
QT_MOC_LITERAL(6, 56, 1), // "x"
QT_MOC_LITERAL(7, 58, 1), // "y"
QT_MOC_LITERAL(8, 60, 8), // "is_right"
QT_MOC_LITERAL(9, 69, 17), // "on_label_released"
QT_MOC_LITERAL(10, 87, 28), // "on_plainTextEdit_textChanged"
QT_MOC_LITERAL(11, 116, 21), // "on_pushButton_pressed"
QT_MOC_LITERAL(12, 138, 10), // "closeEvent"
QT_MOC_LITERAL(13, 149, 12), // "QCloseEvent*"
QT_MOC_LITERAL(14, 162, 5), // "event"
QT_MOC_LITERAL(15, 168, 9), // "hideEvent"
QT_MOC_LITERAL(16, 178, 11), // "QHideEvent*"
QT_MOC_LITERAL(17, 190, 30), // "on_comboBox_currentTextChanged"
QT_MOC_LITERAL(18, 221, 4), // "arg1"
QT_MOC_LITERAL(19, 226, 10), // "wheelEvent"
QT_MOC_LITERAL(20, 237, 12), // "QWheelEvent*"
QT_MOC_LITERAL(21, 250, 32) // "on_comboBox_2_currentTextChanged"

    },
    "Screenshare\0NewImage\0\0img\0slotNewImage\0"
    "on_label_pressed\0x\0y\0is_right\0"
    "on_label_released\0on_plainTextEdit_textChanged\0"
    "on_pushButton_pressed\0closeEvent\0"
    "QCloseEvent*\0event\0hideEvent\0QHideEvent*\0"
    "on_comboBox_currentTextChanged\0arg1\0"
    "wheelEvent\0QWheelEvent*\0"
    "on_comboBox_2_currentTextChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Screenshare[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   72,    2, 0x08 /* Private */,
       5,    3,   75,    2, 0x08 /* Private */,
       9,    3,   82,    2, 0x08 /* Private */,
      10,    0,   89,    2, 0x08 /* Private */,
      11,    0,   90,    2, 0x08 /* Private */,
      12,    1,   91,    2, 0x08 /* Private */,
      15,    1,   94,    2, 0x08 /* Private */,
      17,    1,   97,    2, 0x08 /* Private */,
      19,    1,  100,    2, 0x08 /* Private */,
      21,    1,  103,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPixmap,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QPixmap,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    6,    7,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    6,    7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 16,   14,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void, 0x80000000 | 20,   14,
    QMetaType::Void, QMetaType::QString,   18,

       0        // eod
};

void Screenshare::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Screenshare *_t = static_cast<Screenshare *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->NewImage((*reinterpret_cast< QPixmap(*)>(_a[1]))); break;
        case 1: _t->slotNewImage((*reinterpret_cast< QPixmap(*)>(_a[1]))); break;
        case 2: _t->on_label_pressed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 3: _t->on_label_released((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 4: _t->on_plainTextEdit_textChanged(); break;
        case 5: _t->on_pushButton_pressed(); break;
        case 6: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 7: _t->hideEvent((*reinterpret_cast< QHideEvent*(*)>(_a[1]))); break;
        case 8: _t->on_comboBox_currentTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->wheelEvent((*reinterpret_cast< QWheelEvent*(*)>(_a[1]))); break;
        case 10: _t->on_comboBox_2_currentTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Screenshare::*_t)(QPixmap );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Screenshare::NewImage)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Screenshare::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Screenshare.data,
      qt_meta_data_Screenshare,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Screenshare::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Screenshare::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Screenshare.stringdata0))
        return static_cast<void*>(const_cast< Screenshare*>(this));
    return QDialog::qt_metacast(_clname);
}

int Screenshare::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void Screenshare::NewImage(QPixmap _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
