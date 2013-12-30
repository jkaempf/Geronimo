/****************************************************************************
** Meta object code from reading C++ file 'lightmaps.h'
**
** Created: Mon 30. Dec 17:36:26 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "lightmaps.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lightmaps.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SlippyMap[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   11,   10,   10, 0x05,
      50,   31,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      85,   79,   10,   10, 0x08,
     119,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SlippyMap[] = {
    "SlippyMap\0\0rect\0updated(QRect)\0"
    "latitude,longitude\0positionChanged(qreal,qreal)\0"
    "reply\0handleNetworkData(QNetworkReply*)\0"
    "download()\0"
};

void SlippyMap::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SlippyMap *_t = static_cast<SlippyMap *>(_o);
        switch (_id) {
        case 0: _t->updated((*reinterpret_cast< const QRect(*)>(_a[1]))); break;
        case 1: _t->positionChanged((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 2: _t->handleNetworkData((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 3: _t->download(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SlippyMap::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SlippyMap::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SlippyMap,
      qt_meta_data_SlippyMap, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SlippyMap::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SlippyMap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SlippyMap::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SlippyMap))
        return static_cast<void*>(const_cast< SlippyMap*>(this));
    return QObject::qt_metacast(_clname);
}

int SlippyMap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SlippyMap::updated(const QRect & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SlippyMap::positionChanged(qreal _t1, qreal _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_LightMaps[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      31,   29,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_LightMaps[] = {
    "LightMaps\0\0toggleNightMode()\0r\0"
    "updateMap(QRect)\0"
};

void LightMaps::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LightMaps *_t = static_cast<LightMaps *>(_o);
        switch (_id) {
        case 0: _t->toggleNightMode(); break;
        case 1: _t->updateMap((*reinterpret_cast< const QRect(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LightMaps::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LightMaps::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LightMaps,
      qt_meta_data_LightMaps, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LightMaps::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LightMaps::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LightMaps::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LightMaps))
        return static_cast<void*>(const_cast< LightMaps*>(this));
    return QWidget::qt_metacast(_clname);
}

int LightMaps::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
