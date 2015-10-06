TEMPLATE = app

QT += qml quick widgets

RESOURCES += qml.qrc ../lang.qrc


# Windows specific configuration
win32 {
  include(winconf.pri)
}

# Mac specific configuration
macx {
  include(macxconf.pri)
}

# Unix specific configuration
unix:!macx {
  include(unixconf.pri)
}

include(../qtlibed2k/qtlibed2k.pri)
include(../geoip/geoip.pri)
include(../powermanagement/powermanagement.pri)

INCLUDEPATH += $$PWD/..

HEADERS +=  ../preferences/preferences.h \
            ../misc.h \
            ../qinisettings.h \
            mainwindow.h \
            server_model.h \
            ../res.h

SOURCES +=  ../preferences/preferences.cpp \
            ../misc.cpp \
            main.cpp \
            mainwindow.cpp \
            server_model.cpp

LANG_PATH = ../lang
TRANSLATIONS =  $$LANG_PATH/qDonkey_en.ts \
                $$LANG_PATH/qDonkey_ru.ts
DESTDIR = .

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

include(version.pri)
