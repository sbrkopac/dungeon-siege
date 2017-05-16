
APP = ds
TEMPLATE = app

# setup config defines
CONFIG -= qt
CONFIG += c++11
CONFIG += debug

# This is busted for generating visual studio projects
# The flag ObjectFileName doesn't get properly populated with the following:
# <ObjectFileName>$(IntDir)/%(RelativeDir)/</ObjectFileName>
# This causes all obj files to be dumped to one directory and cause potential clashes
CONFIG += no_batch
CONFIG += object_parallel_to_source

# define all debug specific options - non platform specific
CONFIG (debug, debug|release) {

    DEFINES += _DEBUG

    win32:DESTDIR = bin/x86/debug

    # define all target directories for building
    win32:OBJECTS_DIR = build/x86/debug
}

# define all release specific options - non platform specific
CONFIG (release, debug|release) {

    win32:DESTDIR = bin/x86/release

    # define all target directories for building
    win32:OBJECTS_DIR = build/x86/release
}

unix {

    # HACK: https://bugreports.qt.io/browse/QTCREATORBUG-12541 - gosh dang won't build from kdevelop without this hack -_-
    PKG_CONFIG = PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig pkg-config

    CONFIG += link_pkgconfig

    PKGCONFIG += openscenegraph libxdg-basedir

    LIBS += -lpthread

}

win32 {

    CONFIG += console
    CONFIG -= flat
    DEFINES -= UNICODE

    OSG = $$(OSG_ROOT)
    OSG_LIBS = $$OSG/lib
    OSG_INCLUDES = $$OSG/include

    isEmpty (OSG) {

        error (OpenSceneGraph installation not detected. Please set OSG_ROOT.)
    }

    message ($$OSG)
    message ($$OSG_LIBS)
    message ($$OSG_INCLUDES)

    LIBPATH += $$OSG_LIBS
    INCLUDEPATH += $$OSG_INCLUDES

    # TODO: differentiate between release and debug libraries
    LIBS += OpenThreadsd.lib osgd.lib osgtextd.lib osgviewerd.lib osgGAd.lib osgWidgetd.lib osgDBd.lib osgAnimationd.lib osgUtild.lib shfolder.lib Advapi32.lib
}

include (lib/reverse-engineering-ds.pri)
include (lib/gas.pri)

INCLUDEPATH += lib/minini_12b
SOURCES += lib/minini_12b/minIni.c

INCLUDEPATH += lib/spdlog/include
INCLUDEPATH += lib/gas/source

SOURCES += src/EventProxy.cpp
SOURCES += src/Game.cpp
SOURCES += src/GameStateMgr.cpp
SOURCES += src/InitState.cpp
SOURCES += src/IntroState.cpp
SOURCES += src/LogoState.cpp
SOURCES += src/SimpleConfig.cpp
SOURCES += src/main.cpp
SOURCES += src/FileSys.cpp
SOURCES += src/DirectoryArchive.cpp
SOURCES += src/TankArchive.cpp
SOURCES += src/Logging.cpp
SOURCES += src/ReadFileProxy.cpp