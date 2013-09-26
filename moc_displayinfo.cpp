/****************************************************************************
** Meta object code from reading C++ file 'displayinfo.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "popupviews/displayinfo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'displayinfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DisplayInfo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   13,   12,   12, 0x0a,
      49,   44,   12,   12, 0x0a,
      97,   86,   12,   12, 0x0a,
     127,   12,   12,   12, 0x0a,
     147,   12,   12,   12, 0x0a,
     176,  164,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DisplayInfo[] = {
    "DisplayInfo\0\0row,col\0clickedOnCell(int,int)\0"
    "item\0itemDoubleClicked(QTableWidgetItem*)\0"
    "fileIndex,\0addFileIndex(FileIndex*,uint)\0"
    "removeFileIndexes()\0openGenomeView()\0"
    "item,gbdata\0"
    "receiveSequenceData(QTableWidgetItem*,GeneBrowserData)\0"
};

void DisplayInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DisplayInfo *_t = static_cast<DisplayInfo *>(_o);
        switch (_id) {
        case 0: _t->clickedOnCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->itemDoubleClicked((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->addFileIndex((*reinterpret_cast< FileIndex*(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 3: _t->removeFileIndexes(); break;
        case 4: _t->openGenomeView(); break;
        case 5: _t->receiveSequenceData((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1])),(*reinterpret_cast< GeneBrowserData(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DisplayInfo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DisplayInfo::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DisplayInfo,
      qt_meta_data_DisplayInfo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DisplayInfo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DisplayInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DisplayInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DisplayInfo))
        return static_cast<void*>(const_cast< DisplayInfo*>(this));
    return QDialog::qt_metacast(_clname);
}

int DisplayInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
