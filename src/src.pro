# Global
TEMPLATE = app
CONFIG += qt thread

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

# Windows specific configuration
win32 {
  include(win/conf.pri)  
}

# Mac specific configuration
macx {
  include(mac/conf.pri)
}

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

include(qtlibed2k/qtlibed2k.pri)
include(geoip/geoip.pri)
include(preferences/preferences.pri)
include(powermanagement/powermanagement.pri)

HEADERS +=  misc.h \
            stacktrace.h \
            qinisettings.h \
            res.h \
            fs_utils.h \
            search_widget_delegate.h \
            search_widget_fp_model.h

SOURCES += main.cpp \
           misc.cpp \
            #transferlistdelegate.cpp \
            fs_utils.cpp \
            search_widget_fp_model.cpp

HEADERS +=  mainwindow.h\
          #transferlistwidget.h \
          #transferlistdelegate.h \
          #transferlistfilterswidget.h \
          #torrentcontentmodel.h \
          #torrentcontentmodelitem.h \
          #torrentcontentfiltermodel.h \
          #deletionconfirmationdlg.h \
          ico.h \
          sessionapplication.h \
          iconprovider.h \
          status_bar.h

SOURCES += mainwindow.cpp \
         ico.cpp \
         #transferlistwidget.cpp \
         #torrentcontentmodel.cpp \
         #torrentcontentmodelitem.cpp \
         #torrentcontentfiltermodel.cpp \
         #torrentadditiondlg.cpp \
         sessionapplication.cpp \
         iconprovider.cpp \
         status_bar.cpp

FORMS += mainwindow.ui status_bar.ui
DESTDIR = ../

# OS specific config
OTHER_FILES += win/conf.pri win/conf-msvc.pri mac/conf.pri unix/conf.pri ../version.pri
