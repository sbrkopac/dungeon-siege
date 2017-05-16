
cmake_minimum_required(VERSION 3.7.2)

project(ehb)

set(CMAKE_CXX_STANDARD 11)

if (UNIX)
    find_package(PkgConfig)
    pkg_check_modules(XDGBASEDIR REQUIRED libxdg-basedir)
endif(UNIX)

find_package(OpenGL REQUIRED)
find_package(OpenSceneGraph REQUIRED COMPONENTS osg osgDB osgGA osgViewer)

include_directories(
    lib/minini_12b
    lib/reverse-engineering-dungeon-siege/source
    lib/spdlog/include
    ${OPENGL_INCLUDE_DIRS}
    ${OPENSCENEGRAPH_INCLUDE_DIRS}
)

if (UNIX)
    include_directories(${XDGBASEDIR_INCLUDE_DIRS})
endif(UNIX)

add_executable(ehb
    # minini
    lib/minini_12b/minIni.c
    
    # reverse-engineering-dungeon-siege
    lib/reverse-engineering-dungeon-siege/source/siege/common.cpp
    lib/reverse-engineering-dungeon-siege/source/siege/helper_types.cpp
    lib/reverse-engineering-dungeon-siege/source/siege/tank_file_reader.cpp
    lib/reverse-engineering-dungeon-siege/source/siege/tank_file.cpp
    lib/reverse-engineering-dungeon-siege/source/utils/compression.cpp
    lib/reverse-engineering-dungeon-siege/source/utils/filesys.cpp
    lib/reverse-engineering-dungeon-siege/source/utils/common.cpp
    
    # src
    src/main.cpp
    src/DirectoryArchive.cpp
    src/EventProxy.cpp
    src/FileSys.cpp
    src/Game.cpp
    src/GameStateMgr.cpp
    src/InitState.cpp
    src/IntroState.cpp
    src/Logging.cpp
    src/LogoState.cpp
    src/ReadFileProxy.cpp
    src/SimpleConfig.cpp
    src/TankArchive.cpp
)

target_link_libraries(ehb
    ${OPENGL_LIBRARIES}
    ${OPENSCENEGRAPH_LIBRARIES}
)

if (UNIX)
    target_link_libraries(ehb ${XDGBASEDIR_LIBRARIES} pthread)
endif(UNIX)
