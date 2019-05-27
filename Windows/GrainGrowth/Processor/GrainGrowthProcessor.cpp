#include "GrainGrowthProcessor.h"

bool GrainGrowthProcessor::process(Grid<GrainCell> &grid) {
    if (neighbourhood != Neighbourhood::RADIUS) {
        return BaseGrainGrowthProcessor::process(grid);
    }

    if (coordinatesOfCellsToProcess.empty()) {
        initCellsToProcess(grid);
    }

    CoordinatesVector touchedCellsCoordinates;
    for (const auto &coordinates : coordinatesOfCellsToProcess) {
        auto localTouchedCells = updateZeroCellsInRadius(grid, coordinates);
        touchedCellsCoordinates.insert(touchedCellsCoordinates.end(), localTouchedCells.begin(), localTouchedCells.end());
    }
    coordinatesOfCellsToProcess.clear();

    for (const auto &coordinates : touchedCellsCoordinates) {
        auto &cell = grid[coordinates.first][coordinates.second];
        if (cell.isFake()) {
            continue;
        }
        auto mostFrequentNeighbourCoordinates = cell.getMostFrequentNeighbourCoordinates();
        cell = grid[mostFrequentNeighbourCoordinates.coordinates.first][mostFrequentNeighbourCoordinates.coordinates.second];
        cell.clearNeighbourMap();
        coordinatesOfCellsToProcess.push_back(coordinates);
    }

    return !touchedCellsCoordinates.empty();
}

CoordinatesVector GrainGrowthProcessor::updateZeroCellsInRadius(Grid<GrainCell> &grid, Coordinates cellCoordinates) {
    CoordinatesVector cellsCoordinates;
    auto neighbourCoordinates = getNeighbourhoodService()->getCellNeighbourCoordinates(grid, cellCoordinates.first, cellCoordinates.second);
    for (auto coordinates : neighbourCoordinates) {
        auto isFirstUpdate = grid[coordinates.first][coordinates.second].addNeighbourToMap(grid[cellCoordinates.first][cellCoordinates.second], cellCoordinates);
        if (isFirstUpdate) {
            cellsCoordinates.push_back(coordinates);
        }
    }
    return cellsCoordinates;
}

void GrainGrowthProcessor::initCellsToProcess(Grid<GrainCell> &grid) {
    coordinatesOfCellsToProcess.clear();
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            if (grid[i][j].getState() != 0) {
                coordinatesOfCellsToProcess.emplace_back(i, j);
            }
        }
    }
}

NeighbourhoodService<GrainCell> *GrainGrowthProcessor::getNeighbourhoodService() {
    if (neighbourhood != Neighbourhood::RADIUS) {
        return BaseGrainGrowthProcessor::getNeighbourhoodService();
    }
    return neighbourhoodService->reset()->setNeighbourhood(neighbourhood)->ignoreFakes(true)->setMode(NeighbourhoodService<GrainCell>::ZERO_ONLY)->setRadius(neighbourhoodRadius);
}

GrainGrowthProcessor *GrainGrowthProcessor::setNeighbourhoodRadius(int radius) {
    neighbourhoodRadius = radius;
    return this;
}

void GrainGrowthProcessor::reset() {
    coordinatesOfCellsToProcess.clear();
    BaseGrainGrowthProcessor::reset();
}
