#RC_FILE = donkey.rc

#Adapt the lib names/versions accordingly
CONFIG(debug, debug|release) {
  LIBS += libboost_system-vc140-mt-gd-1_60.lib \
          ed2k.lib
} else {
  LIBS += libboost_system-vc140-mt-1_60.lib \
          ed2k.lib 
}

CONFIG(win7){
  LIBS += ole32.lib
}

QMAKE_LFLAGS += "/OPT:REF /OPT:ICF"

LIBS += advapi32.lib shell32.lib user32.lib
LIBS += PowrProf.lib
