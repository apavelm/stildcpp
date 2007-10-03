TEMPLATE = app
QT = gui core
CONFIG += qt warn_on console debug
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwindow.ui \
 ui/about.ui \
 ui/preferencesdialog.ui \
 ui/PMWindow.ui \
 ui/HUBWindow.ui \
 ui/search.ui \
 ui/favhublist.ui \
 ui/quickconnect.ui
HEADERS = src/mainwindowimpl.h \
 src/frmAbout.h \
 src/mdi_c.h \
 src/preferencesdialog.h \
 src/config.h \
 src/quickconnectdlg.h
SOURCES = src/mainwindowimpl.cpp \
 src/main.cpp \
 src/frmAbout.cpp \
 src/mdi_c.cpp \
 src/preferencesdialog.cpp \
 src/config.cpp \
 src/quickconnectdlg.cpp
RESOURCES += src/systray.qrc
VERNUM = 0.1
