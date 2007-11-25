TEMPLATE = app
QT = gui core xml
CONFIG += qt warn_on console debug
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwin.ui ui/composedlg.ui
HEADERS = ../../src/widgets/zstream.h \
 src/mainwinimpl.h \
 ../../src/widgets/iconset.h \
 ../../src/tabwidget.h \
 src/compose_dlg.h
SOURCES = src/main.cpp \
 ../../src/widgets/zstream.cpp \
 src/mainwinimpl.cpp \
 ../../src/widgets/iconset.cpp \
 ../../src/tabwidget.cpp \
 src/compose_dlg.cpp
RESOURCES += src/icp.qrc
