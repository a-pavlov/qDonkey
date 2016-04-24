# COMPILATION SPECIFIC

QT += androidextras gui-private
QMAKE_CXXFLAGS += -Wformat -Wformat-security -Werror=return-type -Wno-unused-parameter
QMAKE_LFLAGS_APP += -rdynamic
INCLUDEPATH += $$(LIBED2K_ROOT)/include 
INCLUDEPATH += $$(BOOST_ROOT) $$(BOOST_INCLUDEDIR)

CONFIG(debug, debug|release) {
LIBS += -L$$(LIBED2K_ROOT)/bin/debug
} else {
LIBS += -L$$(LIBED2K_ROOT)/bin/release
}

LIBS += -led2k -lboost_thread -lboost_system -lboost_date_time
LIBS += -L$$(BOOST_ROOT)/lib
LIBS += -L$$(BOOST_LIBRARYDIR)

DEFINES += LIBED2K_USE_BOOST_DATE_TIME

CONFIG(debug, debug|release) {
  DEFINES += LIBED2K_DEBUG
} else {
  DEFINES += NDEBUG
 }

DEFINES += "BUILDDATE='\"$$system(date -R)\"'"
DEFINES += BOOST_EXCEPTION_DISABLE


HEADERS += activity.h
SOURCES += activity.cpp


# INSTALL
target.path = $$PREFIX/bin/
INSTALLS += target


DISTFILES += \
    android/AndroidManifest.xml \
    android/AndroidManifest.xml.bak \
    android/AndroidManifest.xml~ \
    android/res/values/libs.xml \
    android/build.gradle \
    android/AndroidManifest.xml~

OTHER_FILES +=  android/src/org/dkfsoft/AMule/AndroidMule.java \
                android/AndroidManifest.xml


ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
