#include <Neighbourhood/Service/TwoDimensionalNeighbourhoodService.h>
#include "GrainGrowthThread.h"

GrainGrowthThread::GrainGrowthThread(Grid<GrainCell> &g, const NeighbourhoodTransferObject<GrainCell> &dto, boost::barrier &b) : grid(g), neighbourhoodTransferObject(dto), barrier(b), neighbourhoodService(new TwoDimensionalNeighbourhoodService<GrainCell>) {}

GrainGrowthThread::GrainGrowthThread(const GrainGrowthThread &other) : grid(other.grid), neighbourhoodTransferObject(other.neighbourhoodTransferObject), barrier(other.barrier), neighbourhoodService(new TwoDimensionalNeighbourhoodService<GrainCell>) {}

GrainGrowthThread::~GrainGrowthThread() {
    delete neighbourhoodService;
}

void GrainGrowthThread::operator()(int index, int chunkSize, int reminder, unsigned short *touched) {
    auto firstElement = index * chunkSize;
    auto countOfCellsToProcess = chunkSize + reminder;
    int counter = 0;

    int i = firstElement / grid.getWidth();
    int j = firstElement % grid.getWidth();
    std::vector<GrainCell *> changedCells;
    for (; i < grid.getHeight(); ++i) {
        for (; j < grid.getWidth() && counter < countOfCellsToProcess; ++j, ++counter) {
            if (grid[i][j].getState() != 0) {
                continue;
            }
            auto mostFrequentNeighbourCell = findMostFrequentNeighbourCell(i, j);
            if (mostFrequentNeighbourCell != nullptr) {
                *touched = 1;
                grid[i][j] = *mostFrequentNeighbourCell;
                changedCells.push_back(&grid[i][j]);
            }
        }
        j = 0;
    }
    barrier.wait();
    for (auto cell : changedCells) {
        cell->resetPreviousState();
    }
}

const GrainCell *GrainGrowthThread::findMostFrequentNeighbourCell(int i, int j) {
    initCellNeighbourMap(i, j);
    auto stateWithCoordinates = grid[i][j].getMostFrequentNeighbourCoordinates();
    grid[i][j].clearNeighbourMap();
    const auto &cell = grid[stateWithCoordinates.coordinates.first][stateWithCoordinates.coordinates.second];
    if (stateWithCoordinates.state == -1 || cell.isFake()) {
        return nullptr;
    }
    return &cell;
}

void GrainGrowthThread::initCellNeighbourMap(int i, int j) {
    auto neighbourCoordinates = neighbourhoodService->getCellNeighbourCoordinates(getNeighbourhoodTransferObject().setCoordinates(Coordinates(i, j)));
    for (const auto &coordinates : neighbourCoordinates) {
        grid[i][j].addStateToMap(grid[coordinates.first][coordinates.second].getPreviousState(), coordinates);
    }
}

NeighbourhoodTransferObject<GrainCell> GrainGrowthThread::getNeighbourhoodTransferObject() {
    return *neighbourhoodTransferObject.reset()->ignoreFakes(true)->setMode(NeighbourhoodTransferObjectMode::NON_ZERO_ONLY);
}
