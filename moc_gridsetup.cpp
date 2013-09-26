/****************************************************************************
** Meta object code from reading C++ file 'gridsetup.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "grid/gridsetup.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gridsetup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GridSetup[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   11,   10,   10, 0x0a,
      38,   11,   10,   10, 0x0a,
      58,   10,   10,   10, 0x0a,
      71,   10,   10,   10, 0x0a,
      92,   10,   10,   10, 0x0a,
     106,   10,   10,   10, 0x0a,
     121,   10,   10,   10, 0x0a,
     134,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GridSetup[] = {
    "GridSetup\0\0selected\0initForm(QString)\0"
    "changeForm(QString)\0addNewGrid()\0"
    "deleteExistingGrid()\0closeWindow()\0"
    "saveAndClose()\0newEC2Form()\0newNonEC2Form()\0"
};

void GridSetup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GridSetup *_t = static_cast<GridSetup *>(_o);
        switch (_id) {
        case 0: _t->initForm((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->changeForm((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->addNewGrid(); break;
        case 3: _t->deleteExistingGrid(); break;
        case 4: _t->closeWindow(); break;
        case 5: _t->saveAndClose(); break;
        case 6: _t->newEC2Form(); break;
        case 7: _t->newNonEC2Form(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GridSetup::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GridSetup::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GridSetup,
      qt_meta_data_GridSetup, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GridSetup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GridSetup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GridSetup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GridSetup))
        return static_cast<void*>(const_cast< GridSetup*>(this));
    return QWidget::qt_metacast(_clname);
}

int GridSetup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
