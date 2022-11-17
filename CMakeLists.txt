cmake_minimum_required(VERSION 3.5)

project(FileIndexGUI VERSION 0.1 LANGUAGES CXX)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LOG_MAXLEVEL 10 CACHE STRING "Set the maximum log level")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

file(GLOB_RECURSE TS_FILES "lang/**.ts")

file(GLOB_RECURSE CXX_SOURCES "gui/src/**.cpp")
file(GLOB_RECURSE HEADERS "gui/include/**.h")
file(GLOB_RECURSE UI_SOURCES "gui/ui/**.ui")

add_subdirectory(FileIndex/Log)
include_directories(FileIndex/Log/include)

add_subdirectory(FileIndex/NamesDB)
include_directories(FileIndex/NamesDB/include)

add_subdirectory(FileIndex/fileindex)
include_directories(FileIndex/fileindex/include)

include_directories(gui/include)

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

set(PROJECT_SOURCES
        ${CXX_SOURCES}
        ${HEADERS}
        ${UI_SOURCES}
        ${QM_FILES}
)

add_executable(FileIndexGUI ${PROJECT_SOURCES})

target_link_libraries(FileIndexGUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets log fileindex)

set_target_properties(FileIndexGUI PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS FileIndexGUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES ${QM_FILES}
    DESTINATION ${CMAKE_INSTALL_DIR}/qm
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FileIndexGUI)
endif()
