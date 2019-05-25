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
        cell = *cell.getMostFrequentNeighbour();
        cell.clearNeighbourMap();
        coordinatesOfCellsToProcess.push_back(coordinates);
    }

    if (touchedCellsCoordinates.empty()) {
        stopSimulation();
    }
    previousState = grid;
}

RadiusNeighbourhoodGridModel::CoordinatesVector RadiusNeighbourhoodGridModel::updateZeroCellsInRadius(int cellY, int cellX) {
    CoordinatesVector cellsCoordinates;
    auto centerOfGravity = grid[cellY][cellX].getCenterOfGravity();
    double cx = cellX + centerOfGravity.first;
    double cy = cellY + centerOfGravity.second;
    for (int y = cellY - neighbourhoodRadius; y < cellY + neighbourhoodRadius; ++y) {
        for (int x = cellX - neighbourhoodRadius; x < cellX + neighbourhoodRadius; ++x) {
            if (grid[y][x].getState() != 0) {
                continue;
            }
            auto currentCellGravityCenter = grid[y][x].getCenterOfGravity();
            double xx = x + currentCellGravityCenter.first;
            double yy = y + currentCellGravityCenter.second;
            auto distance = sqrt(pow(yy - cy, 2) + pow(xx - cx, 2));
            if (distance < neighbourhoodRadius) {
                auto isFirstUpdate = grid[y][x].addNeighbourToMap(grid[cellY][cellX]);
                if (isFirstUpdate) {
                    cellsCoordinates.emplace_back(y, x);
                }
            }
        }
    }
    return cellsCoordinates;
}
