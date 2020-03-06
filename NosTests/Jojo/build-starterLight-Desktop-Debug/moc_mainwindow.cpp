/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../starterLight_testNewErgo/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata0[450];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 32), // "on_pushButton_chargement_clicked"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 34), // "on_pushButton_chargement_2_cl..."
QT_MOC_LITERAL(4, 80, 31), // "on_pushButton_superMesh_clicked"
QT_MOC_LITERAL(5, 112, 37), // "on_pushButton_spherizationSrc..."
QT_MOC_LITERAL(6, 150, 37), // "on_pushButton_spherizationTgt..."
QT_MOC_LITERAL(7, 188, 28), // "on_pushButton_fusion_clicked"
QT_MOC_LITERAL(8, 217, 39), // "on_pushButton_despherizationS..."
QT_MOC_LITERAL(9, 257, 39), // "on_pushButton_despherizationT..."
QT_MOC_LITERAL(10, 297, 23), // "on_spinBox_valueChanged"
QT_MOC_LITERAL(11, 321, 4), // "arg1"
QT_MOC_LITERAL(12, 326, 25), // "on_add_vertex_src_clicked"
QT_MOC_LITERAL(13, 352, 25), // "on_spinBox_2_valueChanged"
QT_MOC_LITERAL(14, 378, 25), // "on_add_vertex_tgt_clicked"
QT_MOC_LITERAL(15, 404, 41), // "on_doubleSpinBox_pourcentage_..."
QT_MOC_LITERAL(16, 446, 3) // "val"

    },
    "MainWindow\0on_pushButton_chargement_clicked\0"
    "\0on_pushButton_chargement_2_clicked\0"
    "on_pushButton_superMesh_clicked\0"
    "on_pushButton_spherizationSrc_clicked\0"
    "on_pushButton_spherizationTgt_clicked\0"
    "on_pushButton_fusion_clicked\0"
    "on_pushButton_despherizationSrc_clicked\0"
    "on_pushButton_despherizationTgt_clicked\0"
    "on_spinBox_valueChanged\0arg1\0"
    "on_add_vertex_src_clicked\0"
    "on_spinBox_2_valueChanged\0"
    "on_add_vertex_tgt_clicked\0"
    "on_doubleSpinBox_pourcentage_valueChanged\0"
    "val"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    1,   87,    2, 0x08 /* Private */,
      12,    0,   90,    2, 0x08 /* Private */,
      13,    1,   91,    2, 0x08 /* Private */,
      14,    0,   94,    2, 0x08 /* Private */,
      15,    1,   95,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   16,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_chargement_clicked(); break;
        case 1: _t->on_pushButton_chargement_2_clicked(); break;
        case 2: _t->on_pushButton_superMesh_clicked(); break;
        case 3: _t->on_pushButton_spherizationSrc_clicked(); break;
        case 4: _t->on_pushButton_spherizationTgt_clicked(); break;
        case 5: _t->on_pushButton_fusion_clicked(); break;
        case 6: _t->on_pushButton_despherizationSrc_clicked(); break;
        case 7: _t->on_pushButton_despherizationTgt_clicked(); break;
        case 8: _t->on_spinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_add_vertex_src_clicked(); break;
        case 10: _t->on_spinBox_2_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_add_vertex_tgt_clicked(); break;
        case 12: _t->on_doubleSpinBox_pourcentage_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
