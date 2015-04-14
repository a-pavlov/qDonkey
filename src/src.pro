# Global
CONFIG += qt thread core gui
TEMPLATE = app

# Store temporary files in separate directories
win32 {
    CONFIG(debug, release|debug):OBJECTS_DIR = tmp/debug
    CONFIG(release, release|debug):OBJECTS_DIR = tmp/release
} else {
    OBJECTS_DIR    = tmp
}

MOC_DIR = $$OBJECTS_DIR/moc
RCC_DIR = $$OBJECTS_DIR/rcc
UI_DIR = $$OBJECTS_DIR/uic

CONFIG(inet) {
    DEFINES += INET_SERVER
}

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    DEFINES += HAVE_QT5
}

# Windows specific configuration
win32 {
  include(win/conf.pri)  
}

# Mac specific configuration
#macx {
#  include(mac/conf.pri)
#}

# Unix specific configuration
unix:!macx {
  include(unix/conf.pri)
}

TARGET = qDonkey
LANG_PATH = lang
ICONS_PATH = icons

CONFIG(debug, debug|release):message(Project is built in DEBUG mode.)
CONFIG(release, debug|release):message(Project is built in RELEASE mode.)

# Disable debug output in release mode
CONFIG(release, debug|release) {
   message(Disabling debug output.)
   DEFINES += QT_NO_DEBUG_OUTPUT
}

# VERSION DEFINES
include(../version.pri)

DEFINES += QT_NO_CAST_TO_ASCII
# Fast concatenation (Qt >= 4.6)
DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

INCLUDEPATH += $$PWD

RESOURCES +=  icons.qrc 

# Source code
usesystemqtsingleapplication {
    CONFIG += qtsingleapplication
} else {
    include(qtsingleapp/qtsingleapplication.pri)
}

# Mac specific configuration
macx {
  include(mac/conf.pri)
}


include(qtlibed2k/qtlibed2k.pri)
include(geoip/geoip.pri)
include(preferences/preferences.pri)
include(powermanagement/powermanagement.pri)
include(search/search.pri)
include(transfers/transfers.pri)
include(statusbar/statusbar.pri)

HEADERS +=  misc.h \
            stacktrace.h \
            qinisettings.h \
            res.h \
            ed2k_link_maker.h \
            collection_save_dlg.h \
    inputdir_dialog.h

SOURCES +=  main.cpp \
            misc.cpp \
            ed2k_link_maker.cpp \
            collection_save_dlg.cpp \
    inputdir_dialog.cpp

HEADERS +=  mainwindow.h\
            ico.h \
            sessionapplication.h \
            iconprovider.h

SOURCES += mainwindow.cpp \
         ico.cpp \
         sessionapplication.cpp \
         iconprovider.cpp

FORMS += mainwindow.ui ed2k_link_maker.ui collection_save_dlg.ui \
    inputdir_dialog.ui

DESTDIR = ../

# OS specific config
OTHER_FILES += win/conf.pri win/conf-msvc.pri mac/conf.pri unix/conf.pri ../version.pri
