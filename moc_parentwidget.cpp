/****************************************************************************
** Meta object code from reading C++ file 'parentwidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "parentwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'parentwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ParentWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      38,   13,   13,   13, 0x08,
      60,   13,   13,   13, 0x08,
      80,   13,   13,   13, 0x08,
      93,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ParentWidget[] = {
    "ParentWidget\0\0continueButtonPressed()\0"
    "cancelButtonPressed()\0backButtonPressed()\0"
    "tabChanged()\0enableContinueButton()\0"
};

void ParentWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ParentWidget *_t = static_cast<ParentWidget *>(_o);
        switch (_id) {
        case 0: _t->continueButtonPressed(); break;
        case 1: _t->cancelButtonPressed(); break;
        case 2: _t->backButtonPressed(); break;
        case 3: _t->tabChanged(); break;
        case 4: _t->enableContinueButton(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ParentWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ParentWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ParentWidget,
      qt_meta_data_ParentWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ParentWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ParentWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ParentWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ParentWidget))
        return static_cast<void*>(const_cast< ParentWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ParentWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
