/****************************************************************************
** Meta object code from reading C++ file 'meganview.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dataviews/meganview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'meganview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MeganView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      20,   10,   10,   10, 0x08,
      36,   30,   10,   10, 0x08,
      56,   50,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MeganView[] = {
    "MeganView\0\0zoomIn()\0zoomOut()\0depth\0"
    "setDepth(int)\0value\0switchStyle(bool)\0"
};

void MeganView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MeganView *_t = static_cast<MeganView *>(_o);
        switch (_id) {
        case 0: _t->zoomIn(); break;
        case 1: _t->zoomOut(); break;
        case 2: _t->setDepth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->switchStyle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MeganView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MeganView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MeganView,
      qt_meta_data_MeganView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MeganView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MeganView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MeganView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MeganView))
        return static_cast<void*>(const_cast< MeganView*>(this));
    return QWidget::qt_metacast(_clname);
}

int MeganView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
