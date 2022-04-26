cmake_minimum_required(VERSION 3.16.0)

set(PRJ_NAME airplane-game)

project(${PRJ_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

add_executable(${PRJ_NAME}
    resource/application.qrc
    src/main.cpp
    src/Airplane.cpp
    src/Bullet.cpp
    src/Enemy.cpp
    src/Game.cpp
    src/Score.cpp
    src/StaticExplosion.cpp
    src/StaticStuff.cpp
    src/CuteSoundPlayer.cpp
)

target_link_libraries(${PRJ_NAME} PRIVATE Qt5::Widgets Qt5::Multimedia)
