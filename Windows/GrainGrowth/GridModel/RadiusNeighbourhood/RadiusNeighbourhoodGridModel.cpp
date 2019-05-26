#include "RadiusNeighbourhoodGridModel.h"

void RadiusNeighbourhoodGridModel::startSimulation(const BoundaryCondition &bc) {
    if (neighbourhood == Neighbourhood::RADIUS) {
        initCellsToProcess();
    }
    GrainGrowthGridModel::startSimulation(bc);
}

void RadiusNeighbourhoodGridModel::initCellsToProcess() {
    coordinatesOfCellsToProcess.clear();
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            if (grid[i][j].getState() != 0) {
                coordinatesOfCellsToProcess.emplace_back(i, j);
            }
        }
    }
}

void RadiusNeighbourhoodGridModel::setNeighbourhoodRadius(int radius) {
    neighbourhoodRadius = radius;
}

void RadiusNeighbourhoodGridModel::simulate() {
    if (neighbourhood != Neighbourhood::RADIUS) {
        GrainGrowthGridModel::simulate();
        return;
    }

    CoordinatesVector touchedCellsCoordinates;
    for (const auto &coordinates : coordinatesOfCellsToProcess) {
        auto localTouchedCells = updateZeroCellsInRadius(coordinates.first, coordinates.second);
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

    if (touchedCellsCoordinates.empty()) {
        simulationEnded();
    }
    previousState = grid;
}

CoordinatesVector RadiusNeighbourhoodGridModel::updateZeroCellsInRadius(int cellY, int cellX) {
    CoordinatesVector cellsCoordinates;
    auto neighbourCoordinates = getNeighbourhoodService()->ignoreFakes(true)->setMode(NeighbourhoodService::ZERO_ONLY)->getCellNeighbourCoordinates(grid, cellY, cellX);
    for (auto coordinates : neighbourCoordinates) {
        auto isFirstUpdate = grid[coordinates.first][coordinates.second].addNeighbourToMap(grid[cellY][cellX], Coordinates(cellY, cellX));
        if (isFirstUpdate) {
            cellsCoordinates.push_back(coordinates);
        }
    }
    return cellsCoordinates;
}

NeighbourhoodService *RadiusNeighbourhoodGridModel::getNeighbourhoodService() {
    return GrainGrowthGridModel::getNeighbourhoodService()->setRadius(neighbourhoodRadius);
}
