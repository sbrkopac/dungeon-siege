
win32 {

    DEFINES += YY_NO_UNISTD_H
}

INCLUDEPATH += lib/gas/source

SOURCES += lib/gas/source/gas.cpp \
           lib/gas/source/gas.l.cpp \
           lib/gas/source/gas.y.cpp