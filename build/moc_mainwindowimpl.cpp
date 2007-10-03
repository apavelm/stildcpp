/****************************************************************************
** Meta object code from reading C++ file 'mainwindowimpl.h'
**
** Created: Wed Oct 3 14:08:36 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/mainwindowimpl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindowimpl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_MainWindowImpl[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      22,   16,   15,   15, 0x08,
      41,   35,   15,   15, 0x08,
      68,   61,   15,   15, 0x08,
     160,  117,   15,   15, 0x08,
     216,  197,   15,   15, 0x28,
     249,   15,   15,   15, 0x08,
     270,  266,   15,   15, 0x08,
     297,  293,   15,   15, 0x08,
     320,   15,   15,   15, 0x08,
     328,   15,   15,   15, 0x08,
     341,   15,   15,   15, 0x08,
     356,   15,   15,   15, 0x08,
     369,   15,   15,   15, 0x08,
     387,   15,   15,   15, 0x08,
     404,   15,   15,   15, 0x08,
     423,  419,   15,   15, 0x08,
     441,   15,   15,   15, 0x08,
     454,   15,   15,   15, 0x08,
     471,   15,   15,   15, 0x08,
     486,   15,   15,   15, 0x08,
     500,   15,   15,   15, 0x08,
     526,  519,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindowImpl[] = {
    "MainWindowImpl\0\0index\0setIcon(int)\0"
    "title\0setToolTip(QString)\0reason\0"
    "iconActivated(QSystemTrayIcon::ActivationReason)\0"
    "title,message,type,millisecondsTimeoutHint\0"
    "showMessage(QString,QString,int,int)\0"
    "title,message,type\0showMessage(QString,QString,int)\0"
    "messageClicked()\0mes\0statusMessage(QString)\0"
    "act\0slotclosemdi(QAction*)\0About()\0"
    "DonateFunc()\0HomepageFunc()\0SearchFunc()\0"
    "PreferencesFunc()\0FavHubListFunc()\0"
    "QuickConFunc()\0adr\0OpenHub(QString&)\0"
    "transcheck()\0statusbarcheck()\0"
    "toolbarcheck()\0winbarcheck()\0"
    "updateWindowMenu()\0window\0"
    "setActiveSubWindow(QWidget*)\0"
};

const QMetaObject MainWindowImpl::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindowImpl,
      qt_meta_data_MainWindowImpl, 0 }
};

const QMetaObject *MainWindowImpl::metaObject() const
{
    return &staticMetaObject;
}

void *MainWindowImpl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindowImpl))
	return static_cast<void*>(const_cast< MainWindowImpl*>(this));
    if (!strcmp(_clname, "Ui::MainWindow"))
	return static_cast< Ui::MainWindow*>(const_cast< MainWindowImpl*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindowImpl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setIcon((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: setToolTip((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: iconActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 3: showMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 4: showMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: messageClicked(); break;
        case 6: statusMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: slotclosemdi((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 8: About(); break;
        case 9: DonateFunc(); break;
        case 10: HomepageFunc(); break;
        case 11: SearchFunc(); break;
        case 12: PreferencesFunc(); break;
        case 13: FavHubListFunc(); break;
        case 14: QuickConFunc(); break;
        case 15: OpenHub((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: transcheck(); break;
        case 17: statusbarcheck(); break;
        case 18: toolbarcheck(); break;
        case 19: winbarcheck(); break;
        case 20: updateWindowMenu(); break;
        case 21: setActiveSubWindow((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        }
        _id -= 22;
    }
    return _id;
}
