#include "GrainGrowthProcessor.h"
#include <mutex>
#include <Windows/GrainGrowth/Processors/GrainGrowth/Threads/RadiusGrainGrowthThread.h>

const int GrainGrowthProcessor::MAX_CELLS_PER_THREAD = 500;
const int GrainGrowthProcessor::MAX_THREAD_COUNT = 12;

bool GrainGrowthProcessor::process(Grid<GrainCell> &grid) {
    if (neighbourhoodTransferObject->getNeighbourhood() != Neighbourhood::RADIUS) {
        return BaseGrainGrowthProcessor::process(grid);
    }

    if (coordinatesOfCellsToProcess.empty()) {
        initCellsToProcess(grid);
    }

    auto cellCount = coordinatesOfCellsToProcess.size();

    int threadCount = cellCount / MAX_CELLS_PER_THREAD;
    threadCount = std::min(threadCount, MAX_THREAD_COUNT);
    threadCount = std::max(threadCount, 1);
    int chunkSize = cellCount / threadCount;
    int reminder = cellCount % threadCount;

    std::vector<CoordinatesVector> results;
    results.assign(threadCount, {});
    std::mutex mutex;
    ThreadVector threads;
    threads.reserve(threadCount);

    for (int i = 0; i < threadCount; ++i) {
        auto currentReminder = i == threadCount - 1 ? reminder : 0;
        threads.emplace_back(RadiusGrainGrowthThread(grid, *neighbourhoodTransferObject, mutex), i, chunkSize, currentReminder, &coordinatesOfCellsToProcess, &results[i]);
    }

    CoordinatesVector coordinatesForNextIteration;
    for (int i = 0; i < threadCount; ++i) {
        threads[i].join();
        coordinatesForNextIteration.insert(coordinatesForNextIteration.end(), results[i].begin(), results[i].end());
    }
    coordinatesOfCellsToProcess = std::move(coordinatesForNextIteration);

    return !coordinatesOfCellsToProcess.empty();
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

void GrainGrowthProcessor::reset() {
    coordinatesOfCellsToProcess.clear();
    BaseGrainGrowthProcessor::reset();
}
