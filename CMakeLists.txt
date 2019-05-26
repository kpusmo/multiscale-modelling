cmake_minimum_required(VERSION 3.12)
project(wieloskalowe)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS UI)

find_package(Qt5Widgets REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(wieloskalowe main.cpp Grid/Grid.h Cells/BinaryCell.h Windows/OneDimensional/OneDimensionalWindow.h Windows/OneDimensional/OneDimensionalWindow.cpp Windows/GrainGrowth/GrainGrowthWindow.h Windows/GrainGrowth/GrainGrowthWindow.cpp Windows/Main/MainWindow.h Windows/Main/MainWindow.cpp Grid/Row.h Cells/BinaryCell.cpp AbstractGridModel/GridModel.h Windows/OneDimensional/GridModel/OneDimensionalGridModel.cpp Windows/OneDimensional/GridModel/OneDimensionalGridModel.h AbstractGridModel/BaseGridModel.h Windows/Game/GameWindow.cpp Windows/Game/GameWindow.h Windows/Game/GridModel/GameGridModel.cpp Windows/Game/GridModel/GameGridModel.h Windows/GrainGrowth/GridModel/Base/GrainGrowthGridModel.cpp Windows/GrainGrowth/GridModel/Base/GrainGrowthGridModel.h Windows/GrainGrowth/GridModel/Base/GrainGrowthGridModel.cpp Windows/GrainGrowth/GridModel/Base/GrainGrowthGridModel.h Cells/GrainCell.cpp Cells/GrainCell.h Grid/BoundaryCondition.h Neighbourhood/Neighbourhood.h Cells/RgbColor.h Windows/GrainGrowth/GridModel/RadiusNeighbourhood/RadiusNeighbourhoodGridModel.cpp Windows/GrainGrowth/GridModel/RadiusNeighbourhood/RadiusNeighbourhoodGridModel.h Windows/GrainGrowth/MonteCarloProcessor/MonteCarloProcessor.cpp Windows/GrainGrowth/MonteCarloProcessor/MonteCarloProcessor.h  Neighbourhood/NeighbourhoodService.h Types/Types.h)

target_link_libraries(wieloskalowe Qt5::Widgets)