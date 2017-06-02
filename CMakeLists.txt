
cmake_minimum_required(VERSION 3.7.2)

project(ehb)

set(CMAKE_CXX_STANDARD 14)

if (UNIX)
    find_package(PkgConfig)
    pkg_check_modules(XDGBASEDIR REQUIRED libxdg-basedir)
endif(UNIX)

find_package(OpenGL REQUIRED)
find_package(OpenSceneGraph REQUIRED COMPONENTS osg osgDB osgGA osgViewer)

find_package(BISON)
find_package(FLEX)

bison_target(GasParser lib/gas/gas.y ${CMAKE_CURRENT_BINARY_DIR}/gas.y.cpp COMPILE_FLAGS "${YACC_FLAGS} --defines=${CMAKE_CURRENT_BINARY_DIR}/gas.y.hpp")
flex_target(GasScanner lib/gas/gas.l ${CMAKE_CURRENT_BINARY_DIR}/gas.l.cpp COMPILE_FLAGS "${LEX_FLAGS} --header-file=${CMAKE_CURRENT_BINARY_DIR}/gas.l.hpp")
add_flex_bison_dependency(GasScanner GasParser)

include_directories(
    lib/minini_12b
    lib/nonstd
    lib/reverse-engineering-dungeon-siege/source
    lib/spdlog/include
    lib/gas
    ${OPENGL_INCLUDE_DIRS}
    ${OPENSCENEGRAPH_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
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
    src/BrkopacState.cpp
    src/DialogBox.cpp
    src/DirectoryArchive.cpp
    src/EventProxy.cpp
    src/FileSys.cpp
    src/Game.cpp
    src/GameStateMgr.cpp
    src/InitState.cpp
    src/IntroState.cpp
    src/LogoState.cpp
    src/ReaderWriterRAW.cpp
    src/ReaderWriterASP.cpp
    src/ReadFileProxy.cpp
    src/SimpleConfig.cpp
    src/TankArchive.cpp
    src/Shell.cpp
    src/Widget.cpp
    src/WidgetBody.cpp
    src/WidgetBorder.cpp
    
    # gas
    lib/gas/gas.cpp
    ${BISON_GasParser_OUTPUTS}
    ${FLEX_GasScanner_OUTPUTS}
)

# disable use of unistd
target_compile_definitions(ehb PRIVATE YY_NO_UNISTD_H GLEW_STATIC)

target_link_libraries(ehb
    ${OPENGL_LIBRARIES}
    ${OPENSCENEGRAPH_LIBRARIES}
)

if (UNIX)
    target_link_libraries(ehb ${XDGBASEDIR_LIBRARIES} pthread stdc++fs)
endif(UNIX)
