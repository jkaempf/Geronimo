######################################################################
# Generated by Jerome Kaempf, LESO-PB, EPFL
######################################################################

# Libraries needed for the compiling (headers) and linking stage
QT+= core \
     gui \
     widgets

greaterThan(QT_MAJOR_VERSION, 5) {
    QT += openglwidgets
}

TARGET = Geronimo
TEMPLATE = app

LIBS += -lopengl32 -lglu32

CONFIG(release, debug|release) {
    DESTDIR = release/bin
    OBJECTS_DIR = release/obj
} else {
    DESTDIR = debug/bin
    OBJECTS_DIR = debug/obj
}

# Mac OS X Icons file
ICON = leso.icns

# Windows Resource File (containing a reference to the icon)
RC_FILE = visuCFS.rc

# Qt's Resource File (containing a reference to a BMP for the window icon)
RESOURCES += visuCFS.qrc

# Headers and so on for the compilation
SOURCES += main.cpp glwidget.cpp visuCFS.cpp radsimul.cpp

HEADERS += glwidget.h visuCFS.h radsimul.h

FORMS += window.ui     
