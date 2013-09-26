/****************************************************************************
** Meta object code from reading C++ file 'tabledata.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dataviews/tabledata.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tabledata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TableData[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      83,   79,   10,   10, 0x0a,
     105,   99,   10,   10, 0x0a,
     151,  124,   10,   10, 0x0a,
     186,  181,   10,   10, 0x0a,
     224,   10,   10,   10, 0x08,
     246,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TableData[] = {
    "TableData\0\0item,gbdata\0"
    "transmitSequenceData(QTableWidgetItem*,GeneBrowserData)\0"
    "top\0outputRows(int)\0index\0headerClicked(int)\0"
    "query,column,caseSensitive\0"
    "searchQuery(QString,int,bool)\0item\0"
    "createGeneViewData(QTableWidgetItem*)\0"
    "searchButtonPressed()\0exportButtonPressed()\0"
};

void TableData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TableData *_t = static_cast<TableData *>(_o);
        switch (_id) {
        case 0: _t->transmitSequenceData((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1])),(*reinterpret_cast< GeneBrowserData(*)>(_a[2]))); break;
        case 1: _t->outputRows((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->headerClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->searchQuery((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 4: _t->createGeneViewData((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 5: _t->searchButtonPressed(); break;
        case 6: _t->exportButtonPressed(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TableData::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TableData::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TableData,
      qt_meta_data_TableData, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TableData::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TableData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TableData::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TableData))
        return static_cast<void*>(const_cast< TableData*>(this));
    if (!strcmp(_clname, "MetaWidget"))
        return static_cast< MetaWidget*>(const_cast< TableData*>(this));
    return QWidget::qt_metacast(_clname);
}

int TableData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void TableData::transmitSequenceData(QTableWidgetItem * _t1, GeneBrowserData _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
