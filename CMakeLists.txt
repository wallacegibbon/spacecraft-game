cmake_minimum_required(VERSION 3.16.0)

set(PRJ_NAME spacecraft-game)

project(${PRJ_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

aux_source_directory(src SRCS)
add_executable(${PRJ_NAME} ${SRCS} resource/application.qrc)
target_link_libraries(${PRJ_NAME} PRIVATE Qt5::Widgets Qt5::Multimedia)
