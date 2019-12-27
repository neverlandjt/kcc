cmake_minimum_required(VERSION 3.11)

project(kcc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(LibArchive REQUIRED)

if(${LibArchive_FOUND})
    include_directories(${LibArchive_INCLUDE_DIRS})
ENDIF(${LibArchive_FOUND})

add_executable(kcc main.cpp mainwindow.cpp mainwindow.h mainwindow.ui archivehandler.cpp archivehandler.h actions.cpp copy_cut_paste.cpp)
set_target_properties(kcc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
target_link_libraries(kcc PRIVATE Qt5::Widgets ${LibArchive_LIBRARIES})
