/****************************************************************************
** Meta object code from reading C++ file 'radsimul.h'
**
** Created: Tue 28. Aug 16:44:46 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "radsimul.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'radsimul.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ShellCommand[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      37,   14,   13,   13, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ShellCommand[] = {
    "ShellCommand\0\0errorState,description\0"
    "taskState(int,QString)\0"
};

void ShellCommand::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ShellCommand *_t = static_cast<ShellCommand *>(_o);
        switch (_id) {
        case 0: _t->taskState((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ShellCommand::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ShellCommand::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ShellCommand,
      qt_meta_data_ShellCommand, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ShellCommand::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ShellCommand::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ShellCommand::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ShellCommand))
        return static_cast<void*>(const_cast< ShellCommand*>(this));
    return QThread::qt_metacast(_clname);
}

int ShellCommand::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ShellCommand::taskState(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_RadianceSimulation[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   20,   19,   19, 0x0a,
      60,   50,   19,   19, 0x0a,
      91,   82,   19,   19, 0x0a,
     118,  112,   19,   19, 0x0a,
     140,  136,   19,   19, 0x0a,
     161,  156,   19,   19, 0x0a,
     190,  174,   19,   19, 0x0a,
     220,  214,   19,   19, 0x0a,
     244,  241,   19,   19, 0x0a,
     270,  262,   19,   19, 0x0a,
     294,  287,   19,   19, 0x0a,
     316,  310,   19,   19, 0x0a,
     339,  310,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RadianceSimulation[] = {
    "RadianceSimulation\0\0latitude\0"
    "setLatitude(QString)\0longitude\0"
    "setLongitude(QString)\0meridian\0"
    "setMeridian(QString)\0month\0setMonth(QString)\0"
    "day\0setDay(QString)\0hour\0setHour(int)\0"
    "siteOrientation\0setSiteOrientation(int)\0"
    "illum\0setIlluminance(bool)\0dl\0"
    "setDaylight(bool)\0mkillum\0setMkillum(bool)\0"
    "prism2\0setPrism2(bool)\0value\0"
    "setDFReqIllum(QString)\0setLogScale(bool)\0"
};

void RadianceSimulation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RadianceSimulation *_t = static_cast<RadianceSimulation *>(_o);
        switch (_id) {
        case 0: _t->setLatitude((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setLongitude((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setMeridian((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setMonth((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->setDay((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->setHour((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setSiteOrientation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setIlluminance((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->setDaylight((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->setMkillum((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->setPrism2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->setDFReqIllum((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->setLogScale((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData RadianceSimulation::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RadianceSimulation::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_RadianceSimulation,
      qt_meta_data_RadianceSimulation, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RadianceSimulation::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RadianceSimulation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RadianceSimulation::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RadianceSimulation))
        return static_cast<void*>(const_cast< RadianceSimulation*>(this));
    return QThread::qt_metacast(_clname);
}

int RadianceSimulation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
