TEMPLATE = app

QT += qml quick widgets svg

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

include(../src/qtlibed2k/qtlibed2k.pri)

INCLUDEPATH += $$PWD/../src

HEADERS +=  ../src/misc.h \
            ../src/qinisettings.h \
            ../src/search/search_widget_fp_model.h \
            mainwindow.h \
            server_model.h \
            search_model.h \
            transfer_model.h \
            transfermodel_item.h \
            preferences.h \
            transferdetailsmodel.h

SOURCES +=  ../src/misc.cpp \
            ../src/search/search_widget_fp_model.cpp \
            main.cpp \
            mainwindow.cpp \
            server_model.cpp \
            search_model.cpp \
            transfer_model.cpp \
            transfermodel_item.cpp \
            preferences.cpp \
            transferdetailsmodel.cpp

LANG_PATH = ../src/lang
TRANSLATIONS =  $$LANG_PATH/qDonkey_en.ts \
                $$LANG_PATH/qDonkey_ru.ts
DESTDIR = .

# Additional import path used to resolve QML modules in Qt Creators code model
QML_IMPORT_PATH =qml

# Default rules for deployment.
include(deployment.pri)

include(version.pri)

#contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
#    ANDROID_EXTRA_LIBS = /home/apavlov/dev/qDonkey/material/../../arm-linux-androideabi_standalone/arm-linux-androideabi/lib/libcrystax.so /home/apavlov/dev/qDonkey/material/../../crystax-ndk-10.2.1/sources/boost/1.58.0/libs/armeabi-v7a/libboost_system.so /home/apavlov/dev/qDonkey/material/../../crystax-ndk-10.2.1/sources/boost/1.58.0/libs/armeabi-v7a/libboost_date_time.so
#}

DISTFILES += \
    android/AndroidManifest.xml \
    android/AndroidManifest.xml.bak \
    android/AndroidManifest.xml~ \
    android/res/values/libs.xml \
    android/build.gradle \
    android/AndroidManifest.xml~

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

