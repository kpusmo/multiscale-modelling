#include <Neighbourhood/Service/TwoDimensionalNeighbourhoodService.h>
#include "RadiusGrainGrowthThread.h"

RadiusGrainGrowthThread::RadiusGrainGrowthThread(Grid<GrainCell> &g, const NeighbourhoodTransferObject<GrainCell> &dto, std::mutex &m) :
    grid(g), neighbourhoodTransferObject(dto), mutex(m), neighbourhoodService(new TwoDimensionalNeighbourhoodService<GrainCell>) {}

RadiusGrainGrowthThread::RadiusGrainGrowthThread(const RadiusGrainGrowthThread &other) :
    grid(other.grid), neighbourhoodTransferObject(other.neighbourhoodTransferObject), mutex(other.mutex), neighbourhoodService(new TwoDimensionalNeighbourhoodService<GrainCell>) {}

RadiusGrainGrowthThread::~RadiusGrainGrowthThread() {
    delete neighbourhoodService;
}

void RadiusGrainGrowthThread::operator()(int index, int chunkSize, int reminder, CoordinatesVector *coordinatesOfCellsToProcess, CoordinatesVector *out) {
    CoordinatesVector touchedCellsCoordinates;
    auto firstElementIndex = index * chunkSize;
    for (int i = firstElementIndex; i < firstElementIndex + chunkSize + reminder; ++i) {
        auto localTouchedCells = updateZeroCellsInRadius((*coordinatesOfCellsToProcess)[i]);
        touchedCellsCoordinates.insert(touchedCellsCoordinates.end(), localTouchedCells.begin(), localTouchedCells.end());
    }

    for (const auto &coordinates : touchedCellsCoordinates) {
        auto &cell = grid[coordinates.first][coordinates.second];
        if (cell.isFake()) {
            continue;
        }
        auto mostFrequentNeighbourCoordinates = cell.getMostFrequentNeighbourCoordinates();
        cell = grid[mostFrequentNeighbourCoordinates.coordinates.first][mostFrequentNeighbourCoordinates.coordinates.second];
        cell.clearNeighbourMap();
        out->push_back(coordinates);
    }
}

CoordinatesVector RadiusGrainGrowthThread::updateZeroCellsInRadius(const Coordinates &cellCoordinates) {
    CoordinatesVector cellsCoordinates;
    auto neighbourCoordinates = neighbourhoodService->getCellNeighbourCoordinates(getNeighbourhoodTransferObject().setCoordinates(cellCoordinates));
    for (auto coordinates : neighbourCoordinates) {
        mutex.lock();
        auto isFirstUpdate = grid[coordinates.first][coordinates.second].addStateToMap(grid[cellCoordinates.first][cellCoordinates.second].getState(), cellCoordinates);
        mutex.unlock();
        if (isFirstUpdate) {
            cellsCoordinates.push_back(coordinates);
        }
    }
    return cellsCoordinates;
}

NeighbourhoodTransferObject<GrainCell> RadiusGrainGrowthThread::getNeighbourhoodTransferObject() {
    return *neighbourhoodTransferObject.reset()->ignoreFakes(true)->setMode(NeighbourhoodTransferObjectMode::ZERO_ONLY);
}
