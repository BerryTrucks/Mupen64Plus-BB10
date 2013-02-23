/****************************************************************************
** Meta object code from reading C++ file 'starshipsettingsapp.h'
**
** Created: Sat Jan 26 13:59:45 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/starshipsettingsapp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'starshipsettingsapp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StarshipSettingsApp[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       3,  114, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,
      41,   20,   20,   20, 0x05,
      59,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      77,   20,   20,   20, 0x0a,
      97,   20,   20,   20, 0x0a,
     119,   20,   20,   20, 0x0a,

 // methods: signature, parameters, type, tag, flags
     166,  142,  134,   20, 0x02,
     217,  195,   20,   20, 0x02,
     266,  247,   20,   20, 0x02,
     330,  307,  134,   20, 0x02,
     384,  378,   20,   20, 0x02,
     404,   20,   20,   20, 0x02,
     423,   20,   20,   20, 0x02,
     450,  437,  433,   20, 0x02,
     497,  477,   20,   20, 0x02,
     528,   20,   20,   20, 0x02,
     540,   20,   20,   20, 0x02,
     552,   20,   20,   20, 0x02,
     571,   20,  433,   20, 0x02,
     583,   20,   20,   20, 0x02,

 // properties: name, type, flags
     598,  134, 0x0a495103,
     602,  433, 0x02495103,
     608,  433, 0x02495103,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

static const char qt_meta_stringdata_StarshipSettingsApp[] = {
    "StarshipSettingsApp\0\0romChanged(QString)\0"
    "videoChanged(int)\0audioChanged(int)\0"
    "addCheatToggle(int)\0addCheatDropDown(int)\0"
    "onManualExit()\0QString\0objectName,defaultValue\0"
    "getValueFor(QString,QString)\0"
    "objectName,inputValue\0"
    "saveValueFor(QString,QString)\0"
    "section,name,value\0"
    "saveConfigValue(QString,QString,QString)\0"
    "rom,section,name,value\0"
    "getConfigValue(QString,QString,QString,QString)\0"
    "start\0startEmulator(bool)\0createCheatsPage()\0"
    "LoadRom()\0int\0player,value\0"
    "getInputValue(int,QString)\0"
    "player,button,value\0setInputValue(int,QString,int)\0"
    "SaveState()\0LoadState()\0LoadTouchOverlay()\0"
    "mapButton()\0ExitEmulator()\0rom\0video\0"
    "audio\0"
};

void StarshipSettingsApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        StarshipSettingsApp *_t = static_cast<StarshipSettingsApp *>(_o);
        switch (_id) {
        case 0: _t->romChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->videoChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->audioChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->addCheatToggle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->addCheatDropDown((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onManualExit(); break;
        case 6: { QString _r = _t->getValueFor((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 7: _t->saveValueFor((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: _t->saveConfigValue((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 9: { QString _r = _t->getConfigValue((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 10: _t->startEmulator((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->createCheatsPage(); break;
        case 12: _t->LoadRom(); break;
        case 13: { int _r = _t->getInputValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 14: _t->setInputValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 15: _t->SaveState(); break;
        case 16: _t->LoadState(); break;
        case 17: _t->LoadTouchOverlay(); break;
        case 18: { int _r = _t->mapButton();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 19: _t->ExitEmulator(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData StarshipSettingsApp::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject StarshipSettingsApp::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_StarshipSettingsApp,
      qt_meta_data_StarshipSettingsApp, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StarshipSettingsApp::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StarshipSettingsApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StarshipSettingsApp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StarshipSettingsApp))
        return static_cast<void*>(const_cast< StarshipSettingsApp*>(this));
    return QThread::qt_metacast(_clname);
}

int StarshipSettingsApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = getRom(); break;
        case 1: *reinterpret_cast< int*>(_v) = getVideo(); break;
        case 2: *reinterpret_cast< int*>(_v) = getAudio(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setRom(*reinterpret_cast< QString*>(_v)); break;
        case 1: setVideo(*reinterpret_cast< int*>(_v)); break;
        case 2: setAudio(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void StarshipSettingsApp::romChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void StarshipSettingsApp::videoChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void StarshipSettingsApp::audioChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
