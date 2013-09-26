/****************************************************************************
** Meta object code from reading C++ file 'genomeview.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "genebrowser/genomeview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'genomeview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GenomeView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GenomeView[] = {
    "GenomeView\0\0item\0itemGenomeDisplay(QTableWidgetItem*)\0"
};

void GenomeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GenomeView *_t = static_cast<GenomeView *>(_o);
        switch (_id) {
        case 0: _t->itemGenomeDisplay((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GenomeView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GenomeView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_GenomeView,
      qt_meta_data_GenomeView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GenomeView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GenomeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GenomeView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GenomeView))
        return static_cast<void*>(const_cast< GenomeView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int GenomeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
