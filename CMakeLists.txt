cmake_minimum_required(VERSION 3.12)
project(wieloskalowe)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC_SEARCH_PATHS  UI)

include_directories(${CMAKE_BUILD_RPATH}/${PROJECT_NAME}_autogen/include Headers)
find_package(Qt5Widgets REQUIRED)

add_executable(wieloskalowe main.cpp Headers/Grid.h Headers/Cell.h Headers/MainWindow.h Sources/MainWindow.cpp Headers/Row.h Headers/SimulationController.h Sources/SimulationController.cpp Headers/Box.h Sources/Box.cpp Sources/Row.cpp Sources/Cell.cpp Sources/Grid.cpp)

target_link_libraries(wieloskalowe Qt5::Widgets)

set(AUTOUIC ON)