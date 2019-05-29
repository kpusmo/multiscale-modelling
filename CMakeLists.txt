cmake_minimum_required(VERSION 3.12)
project(wieloskalowe)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS UI)

find_package (Threads)
find_package(Qt5Widgets REQUIRED)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.45.0 COMPONENTS thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(wieloskalowe main.cpp Grid/Grid.h Cells/BinaryCell.h Windows/OneDimensional/OneDimensionalWindow.h Windows/OneDimensional/OneDimensionalWindow.cpp Windows/GrainGrowth/GrainGrowthWindow.h Windows/GrainGrowth/GrainGrowthWindow.cpp Windows/Main/MainWindow.h Windows/Main/MainWindow.cpp Grid/Row.h Cells/BinaryCell.cpp AbstractGridModel/GridModel.h Windows/OneDimensional/GridModel/OneDimensionalGridModel.cpp Windows/OneDimensional/GridModel/OneDimensionalGridModel.h AbstractGridModel/BaseGridModel.h Windows/Game/GameWindow.cpp Windows/Game/GameWindow.h Windows/Game/GridModel/GameGridModel.cpp Windows/Game/GridModel/GameGridModel.h Windows/GrainGrowth/GridModel/GrainGrowthGridModel.cpp Windows/GrainGrowth/GridModel/GrainGrowthGridModel.h Windows/GrainGrowth/GridModel/GrainGrowthGridModel.cpp Windows/GrainGrowth/GridModel/GrainGrowthGridModel.h Cells/GrainCell.cpp Cells/GrainCell.h Grid/BoundaryCondition.h Neighbourhood/Neighbourhood.h Cells/RgbColor.h PostProcessors/MonteCarlo/MonteCarloProcessor.cpp PostProcessors/MonteCarlo/MonteCarloProcessor.h Neighbourhood/Service/NeighbourhoodService.h Types/Types.h Windows/GrainGrowth/Processor/BaseGrainGrowthProcessor.cpp Windows/GrainGrowth/Processor/BaseGrainGrowthProcessor.h AbstractProcessor/Processor.h Windows/Game/Processor/GameProcessor.cpp Windows/Game/Processor/GameProcessor.h Windows/GrainGrowth/Processor/GrainGrowthProcessor.cpp Windows/GrainGrowth/Processor/GrainGrowthProcessor.h Windows/OneDimensional/Processor/OneDimensionalProcessor.cpp Windows/OneDimensional/Processor/OneDimensionalProcessor.h Neighbourhood/Service/TwoDimensionalNeighbourhoodService.h Neighbourhood/Service/OneDimensionalNeighbourhoodService.h Windows/GrainGrowth/Processor/Threads/GrainGrowthThread.cpp Windows/GrainGrowth/Processor/Threads/GrainGrowthThread.h Windows/GrainGrowth/Processor/Threads/RadiusGrainGrowthThread.cpp Windows/GrainGrowth/Processor/Threads/RadiusGrainGrowthThread.h PostProcessors/MonteCarlo/Thread/MonteCarloThread.cpp PostProcessors/MonteCarlo/Thread/MonteCarloThread.h Neighbourhood/DataTransferObject/NeighbourhoodTransferObject.h)

target_link_libraries(wieloskalowe Qt5::Widgets ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})