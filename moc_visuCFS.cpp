/****************************************************************************
** Meta object code from reading C++ file 'visuCFS.h'
**
** Created: Sun 26. Jan 19:19:24 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "visuCFS.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'visuCFS.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MaForme[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      25,   19,    8,    8, 0x0a,
      65,   46,    8,    8, 0x0a,
      90,    8,    8,    8, 0x0a,
     104,   19,    8,    8, 0x0a,
     127,   19,    8,    8, 0x0a,
     151,   19,    8,    8, 0x0a,
     175,   19,    8,    8, 0x0a,
     198,   19,    8,    8, 0x0a,
     215,   19,    8,    8, 0x0a,
     244,  239,    8,    8, 0x0a,
     272,   19,    8,    8, 0x0a,
     292,   19,    8,    8, 0x0a,
     312,   19,    8,    8, 0x0a,
     333,   19,    8,    8, 0x0a,
     353,   19,    8,    8, 0x0a,
     377,    8,    8,    8, 0x0a,
     404,   19,    8,    8, 0x0a,
     422,    8,    8,    8, 0x0a,
     446,    8,    8,    8, 0x0a,
     477,    8,    8,    8, 0x0a,
     490,    8,    8,    8, 0x0a,
     502,    8,    8,    8, 0x0a,
     519,    8,    8,    8, 0x0a,
     533,   19,    8,    8, 0x0a,
     563,    8,    8,    8, 0x0a,
     583,    8,    8,    8, 0x0a,
     612,    8,    8,    8, 0x0a,
     634,    8,    8,    8, 0x0a,
     672,  649,    8,    8, 0x0a,
     702,    8,    8,    8, 0x0a,
     718,    8,    8,    8, 0x0a,
     752,  742,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MaForme[] = {
    "MaForme\0\0getPath()\0value\0setModelChanged(int)\0"
    "latitude,longitude\0setPosition(qreal,qreal)\0"
    "getPosition()\0setPredefinedDay(bool)\0"
    "setSummerSolstice(bool)\0setWinterSolstice(bool)\0"
    "setSpringEquinox(bool)\0setHour(QString)\0"
    "setSiteOrientation(int)\0text\0"
    "setSiteOrientation(QString)\0"
    "setModeChanged(int)\0setFalseColor(bool)\0"
    "setIlluminance(bool)\0setBluminance(bool)\0"
    "setDaylightFactor(bool)\0"
    "setDFRequiredIlluminance()\0setLogScale(bool)\0"
    "setRadianceParameters()\0"
    "setRadianceParametersMkillum()\0"
    "setQuality()\0setDetail()\0setVariability()\0"
    "setIndirect()\0setGlareRiskCalculation(bool)\0"
    "setGlareRiskIndex()\0radianceSimulation_clicked()\0"
    "convertBTDF_clicked()\0freezeWindow()\0"
    "errorState,description\0"
    "displayTaskState(int,QString)\0"
    "releaseWindow()\0loadSimulationResults()\0"
    "textValue\0setMaxValue(QString)\0"
};

void MaForme::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MaForme *_t = static_cast<MaForme *>(_o);
        switch (_id) {
        case 0: _t->getPath(); break;
        case 1: _t->setModelChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setPosition((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 3: _t->getPosition(); break;
        case 4: _t->setPredefinedDay((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setSummerSolstice((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setWinterSolstice((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->setSpringEquinox((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->setHour((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->setSiteOrientation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->setSiteOrientation((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->setModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->setFalseColor((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->setIlluminance((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->setBluminance((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->setDaylightFactor((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->setDFRequiredIlluminance(); break;
        case 17: _t->setLogScale((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: _t->setRadianceParameters(); break;
        case 19: _t->setRadianceParametersMkillum(); break;
        case 20: _t->setQuality(); break;
        case 21: _t->setDetail(); break;
        case 22: _t->setVariability(); break;
        case 23: _t->setIndirect(); break;
        case 24: _t->setGlareRiskCalculation((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: _t->setGlareRiskIndex(); break;
        case 26: _t->radianceSimulation_clicked(); break;
        case 27: _t->convertBTDF_clicked(); break;
        case 28: _t->freezeWindow(); break;
        case 29: _t->displayTaskState((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 30: _t->releaseWindow(); break;
        case 31: _t->loadSimulationResults(); break;
        case 32: _t->setMaxValue((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MaForme::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MaForme::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MaForme,
      qt_meta_data_MaForme, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MaForme::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MaForme::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MaForme::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MaForme))
        return static_cast<void*>(const_cast< MaForme*>(this));
    return QWidget::qt_metacast(_clname);
}

int MaForme::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
