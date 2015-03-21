RC_FILE = donkey.rc

# Enable Wide characters
DEFINES += TORRENT_USE_WPATH

#Adapt the lib names/versions accordingly
CONFIG(debug, debug|release) {
  LIBS += libboost_system-vc90-mt-gd-1_55.lib \
          #libboost_date_time-vc90-mt-gd-1_55.lib \
          libed2k.lib
          #libeay32MD.lib \
          #ssleay32MD.lib
} else {
  LIBS += libboost_system-vc90-mt-1_55.lib \
          libboost_date_time-vc90-mt-1_55.lib \ 
          libed2k.lib 
          #libeay32MD.lib \
          #ssleay32MD.lib
}

CONFIG(win7){
  LIBS += ole32.lib
}

QMAKE_LFLAGS += "/OPT:REF /OPT:ICF"

LIBS += advapi32.lib shell32.lib user32.lib
LIBS += PowrProf.lib
