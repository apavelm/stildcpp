/****************************************************************************
** Meta object code from reading C++ file 'mdi_c.h'
**
** Created: Wed Oct 3 12:05:49 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/mdi_c.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mdi_c.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_MdiChild[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   10,    9,    9, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_MdiChild[] = {
    "MdiChild\0\0action\0actionReleased(QAction*)\0"
};

const QMetaObject MdiChild::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MdiChild,
      qt_meta_data_MdiChild, 0 }
};

const QMetaObject *MdiChild::metaObject() const
{
    return &staticMetaObject;
}

void *MdiChild::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MdiChild))
	return static_cast<void*>(const_cast< MdiChild*>(this));
    return QDialog::qt_metacast(_clname);
}

int MdiChild::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: actionReleased((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void MdiChild::actionReleased(QAction * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_PMWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_PMWindow[] = {
    "PMWindow\0"
};

const QMetaObject PMWindow::staticMetaObject = {
    { &MdiChild::staticMetaObject, qt_meta_stringdata_PMWindow,
      qt_meta_data_PMWindow, 0 }
};

const QMetaObject *PMWindow::metaObject() const
{
    return &staticMetaObject;
}

void *PMWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PMWindow))
	return static_cast<void*>(const_cast< PMWindow*>(this));
    return MdiChild::qt_metacast(_clname);
}

int PMWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MdiChild::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_HubWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_HubWindow[] = {
    "HubWindow\0"
};

const QMetaObject HubWindow::staticMetaObject = {
    { &MdiChild::staticMetaObject, qt_meta_stringdata_HubWindow,
      qt_meta_data_HubWindow, 0 }
};

const QMetaObject *HubWindow::metaObject() const
{
    return &staticMetaObject;
}

void *HubWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HubWindow))
	return static_cast<void*>(const_cast< HubWindow*>(this));
    return MdiChild::qt_metacast(_clname);
}

int HubWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MdiChild::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_SearchWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SearchWindow[] = {
    "SearchWindow\0\0SearchBtn()\0"
};

const QMetaObject SearchWindow::staticMetaObject = {
    { &MdiChild::staticMetaObject, qt_meta_stringdata_SearchWindow,
      qt_meta_data_SearchWindow, 0 }
};

const QMetaObject *SearchWindow::metaObject() const
{
    return &staticMetaObject;
}

void *SearchWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SearchWindow))
	return static_cast<void*>(const_cast< SearchWindow*>(this));
    return MdiChild::qt_metacast(_clname);
}

int SearchWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MdiChild::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SearchBtn(); break;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_FavoriteHubListWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_FavoriteHubListWindow[] = {
    "FavoriteHubListWindow\0"
};

const QMetaObject FavoriteHubListWindow::staticMetaObject = {
    { &MdiChild::staticMetaObject, qt_meta_stringdata_FavoriteHubListWindow,
      qt_meta_data_FavoriteHubListWindow, 0 }
};

const QMetaObject *FavoriteHubListWindow::metaObject() const
{
    return &staticMetaObject;
}

void *FavoriteHubListWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FavoriteHubListWindow))
	return static_cast<void*>(const_cast< FavoriteHubListWindow*>(this));
    return MdiChild::qt_metacast(_clname);
}

int FavoriteHubListWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MdiChild::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
