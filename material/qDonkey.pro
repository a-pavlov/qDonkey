TEMPLATE = app

QT += qml quick widgets svg multimedia network

RESOURCES += qml.qrc ../src/lang.qrc \
    images.qrc

DEFINES += IS74
QTPLUGN += qsvg

# Windows specific configuration
win32 {
  include(winconf.pri)
}

# Mac specific configuration
macx {
  include(macxconf.pri)
}

# Unix specific configuration
unix:!android {
  include(unixconf.pri)
}

android {
  include(android.pri)
}

CONFIG(debug, debug|release) {
    DEFINES += LIBED2K_DHT_VERBOSE_LOGGING
}

include(../src/qtlibed2k/qtlibed2k.pri)

INCLUDEPATH += $$PWD/../src

HEADERS +=  ../src/misc.h \
            ../src/qinisettings.h \
            search_widget_fp_model.h \
            mainwindow.h \
            server_model.h \
            search_model.h \
            transfer_model.h \
            transfermodel_item.h \
            preferences.h \
            transferdetailsmodel.h \
            transfersortfiltermodel.h \
            peermodel.h \
            kadnodes.h \
            file_downloader.h

SOURCES +=  ../src/misc.cpp \
            search_widget_fp_model.cpp \
            main.cpp \
            mainwindow.cpp \
            server_model.cpp \
            search_model.cpp \
            transfer_model.cpp \
            transfermodel_item.cpp \
            preferences.cpp \
            transferdetailsmodel.cpp \
            transfersortfiltermodel.cpp \
            peermodel.cpp \
            kadnodes.cpp \
            file_downloader.cpp

lupdate_only {
    SOURCES += qml/Connection.qml \
    qml/Donkey.qml \
    qml/InitDialog.qml \
    qml/Preferences.qml \
    qml/Search.qml \
    qml/SearchResult.qml \
    qml/TransferDetails.qml \
    qml/Transfers.qml \
    qml/Kademlia.qml
}

LANG_PATH = ../src/lang
TRANSLATIONS =  $$LANG_PATH/qDonkey_en.ts \
                $$LANG_PATH/qDonkey_ru.ts
DESTDIR = .

# Additional import path used to resolve QML modules in Qt Creators code model
QML_IMPORT_PATH =qml

# Default rules for deployment.
include(deployment.pri)

include(version.pri)


DISTFILES += \
    android/AndroidManifest.xml \
    android/AndroidManifest.xml.bak \
    android/AndroidManifest.xml~ \
    android/res/values/libs.xml \
    android/build.gradle \
    android/AndroidManifest.xml~

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

