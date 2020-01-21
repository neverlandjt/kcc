cmake_minimum_required(VERSION 3.11)

project(kcc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(LibArchive REQUIRED)

if(${LibArchive_FOUND})
    include_directories(${LibArchive_INCLUDE_DIRS})
ENDIF(${LibArchive_FOUND})

add_executable(kcc src/main.cpp src/mainwindow.cpp inc/mainwindow.h src/mainwindow.ui src/archivehandler.cpp
    inc/archivehandler.h src/actions.cpp src/copypaste.cpp inc/finder.h src/finder.ui src/finder.cpp)
set_target_properties(kcc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
target_link_libraries(kcc PRIVATE Qt5::Widgets ${LibArchive_LIBRARIES})
