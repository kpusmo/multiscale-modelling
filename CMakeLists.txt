cmake_minimum_required(VERSION 3.12)
project(wieloskalowe)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS  UI)

find_package(Qt5Widgets REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(Headers)

add_executable(wieloskalowe main.cpp Headers/Grid.h Headers/Cell.h Headers/MainWindow.h Sources/MainWindow.cpp Headers/Row.h Sources/Row.cpp Sources/Cell.cpp Sources/Grid.cpp Sources/GridModel.cpp Headers/GridModel.h)

target_link_libraries(wieloskalowe Qt5::Widgets)