cmake_minimum_required(VERSION 3.12)
project(wieloskalowe)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS UI)

find_package(Qt5Widgets REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(wieloskalowe main.cpp Grid/Grid.h Grid/Cell.h Windows/OneDimensional/OneDimensionalWindow.h Windows/OneDimensional/OneDimensionalWindow.cpp Windows/Main/MainWindow.h Windows/Main/MainWindow.cpp Grid/Row.h Grid/Cell.cpp AbstractGridModel/GridModel.h Windows/OneDimensional/GridModel/OneDimensionalGridModel.cpp Windows/OneDimensional/GridModel/OneDimensionalGridModel.h AbstractGridModel/BaseGridModel.h Windows/Game/GameWindow.cpp Windows/Game/GameWindow.h Windows/Game/GridModel/GameGridModel.cpp Windows/Game/GridModel/GameGridModel.h)

target_link_libraries(wieloskalowe Qt5::Widgets)