/****************************************************************************
** Meta object code from reading C++ file 'RunConfig.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "RunConfig.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RunConfig.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RunConfig[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      21,   10,   10,   10, 0x08,
      35,   10,   10,   10, 0x08,
      50,   10,   10,   10, 0x08,
      66,   10,   10,   10, 0x08,
      82,   10,   10,   10, 0x08,
      95,   10,   10,   10, 0x08,
     110,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RunConfig[] = {
    "RunConfig\0\0fileSet()\0closeWindow()\0"
    "toggleAllRun()\0toggleAllSkip()\0"
    "toggleAllRedo()\0browseFile()\0"
    "browseFolder()\0specifyGrid()\0"
};

void RunConfig::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RunConfig *_t = static_cast<RunConfig *>(_o);
        switch (_id) {
        case 0: _t->fileSet(); break;
        case 1: _t->closeWindow(); break;
        case 2: _t->toggleAllRun(); break;
        case 3: _t->toggleAllSkip(); break;
        case 4: _t->toggleAllRedo(); break;
        case 5: _t->browseFile(); break;
        case 6: _t->browseFolder(); break;
        case 7: _t->specifyGrid(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData RunConfig::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RunConfig::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RunConfig,
      qt_meta_data_RunConfig, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RunConfig::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RunConfig::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RunConfig::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RunConfig))
        return static_cast<void*>(const_cast< RunConfig*>(this));
    return QWidget::qt_metacast(_clname);
}

int RunConfig::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void RunConfig::fileSet()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
