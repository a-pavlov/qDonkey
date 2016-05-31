CONFIG(debug, debug|release) {
  LIBS += libboost_system-vc140-mt-gd-1_60.lib \
          ed2k.lib
} else {
  LIBS += libboost_system-vc140-mt-1_60.lib \
          ed2k.lib 
}

QMAKE_LFLAGS += "/OPT:REF /OPT:ICF"

LIBS += advapi32.lib
