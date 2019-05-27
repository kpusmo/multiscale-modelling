#include "BaseGrainGrowthProcessor.h"
#include <Neighbourhood/TwoDimensionalNeighbourhoodService.h>

BaseGrainGrowthProcessor::BaseGrainGrowthProcessor() : Processor<GrainCell>(new TwoDimensionalNeighbourhoodService<GrainCell>) {}

bool BaseGrainGrowthProcessor::process(Grid<GrainCell> &grid) {
    if (previousState.getHeight() == 0) {
        previousState = grid;
    }
    bool touched = false;
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            if (grid[i][j].getState() != 0) {
                continue;
            }
            auto mostFrequentNeighbourCell = findMostFrequentNeighbourCell(grid, i, j);
            if (mostFrequentNeighbourCell != nullptr) {
                touched = true;
                grid[i][j] = *mostFrequentNeighbourCell;
            }
        }
    }
    previousState = grid;
    return touched;
}

const GrainCell *BaseGrainGrowthProcessor::findMostFrequentNeighbourCell(Grid<GrainCell> &grid, int i, int j) {
    initCellNeighbourMap(grid, i, j);
    auto coordinates = grid[i][j].getMostFrequentNeighbourCoordinates();
    grid[i][j].clearNeighbourMap();
    const auto &cell = grid[coordinates.coordinates.first][coordinates.coordinates.second];
    if (coordinates.state == -1 || cell.isFake()) {
        return nullptr;
    }
    return &grid[coordinates.coordinates.first][coordinates.coordinates.second];
}

void BaseGrainGrowthProcessor::initCellNeighbourMap(Grid<GrainCell> &grid, int i, int j) {
    auto neighbourCoordinates = getNeighbourhoodService()->getCellNeighbourCoordinates(grid, i, j);
    for (const auto &coordinates : neighbourCoordinates) {
        grid[i][j].addNeighbourToMap(previousState[coordinates.first][coordinates.second], coordinates);
    }
}

NeighbourhoodService<GrainCell> *BaseGrainGrowthProcessor::getNeighbourhoodService() {
    return neighbourhoodService->reset()->setNeighbourhood(neighbourhood)->ignoreFakes(true)->setMode(NeighbourhoodService<GrainCell>::NON_ZERO_ONLY);
}

void BaseGrainGrowthProcessor::reset() {
    previousState.reset(0, 0);
}
