/****************************************************************************
** Meta object code from reading C++ file 'SettingsTab.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "SettingsTab.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SettingsTab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SettingsTab[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      33,   12,   12,   12, 0x08,
      47,   12,   12,   12, 0x08,
      68,   12,   12,   12, 0x08,
      91,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SettingsTab[] = {
    "SettingsTab\0\0setContinueButton()\0"
    "closeWindow()\0openParameterSetup()\0"
    "annotationDBSPressed()\0rrnaREFDBSPressed()\0"
};

void SettingsTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SettingsTab *_t = static_cast<SettingsTab *>(_o);
        switch (_id) {
        case 0: _t->setContinueButton(); break;
        case 1: _t->closeWindow(); break;
        case 2: _t->openParameterSetup(); break;
        case 3: _t->annotationDBSPressed(); break;
        case 4: _t->rrnaREFDBSPressed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData SettingsTab::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SettingsTab::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SettingsTab,
      qt_meta_data_SettingsTab, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SettingsTab::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SettingsTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SettingsTab::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SettingsTab))
        return static_cast<void*>(const_cast< SettingsTab*>(this));
    return QWidget::qt_metacast(_clname);
}

int SettingsTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SettingsTab::setContinueButton()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
