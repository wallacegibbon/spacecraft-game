cmake_minimum_required(VERSION 3.16.0)

set(PRJ_NAME airplane-game)

project(${PRJ_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

qt_add_executable(${PRJ_NAME} src/main.cpp src/Airplane.cpp src/Bullet.cpp src/Enemy.cpp)
target_link_libraries(${PRJ_NAME} PRIVATE Qt6::Widgets)
