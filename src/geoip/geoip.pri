INCLUDEPATH += $$PWD

HEADERS +=  $$PWD/geoipmanager.h
SOURCES +=  $$PWD/geoipmanager.cpp

#unix {
    HEADERS += $$PWD/GeoIP.h
    SOURCES += $$PWD/GeoIP.c
#}


# Add GeoIP resource file if the GeoIP database
# should be embedded in qBittorrent executable
contains(DEFINES, WITH_GEOIP_EMBEDDED) {
  exists("GeoIP.dat") {
    message("GeoIP.dat was found in src/geoip/.")
    RESOURCES += $$PWD/geoip.qrc
  } else {
    DEFINES -= WITH_GEOIP_EMBEDDED
    error("GeoIP.dat was not found in src/geoip/ folder, please follow instructions in src/geoip/README.")
  }
} else {
  message("GeoIP database will not be embedded in new-mule executable.")
}
