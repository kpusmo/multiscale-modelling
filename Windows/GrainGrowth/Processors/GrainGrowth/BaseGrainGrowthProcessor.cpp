#include "BaseGrainGrowthProcessor.h"
#include <Neighbourhood/Service/TwoDimensionalNeighbourhoodService.h>
#include <Windows/GrainGrowth/Processors/GrainGrowth/Threads/GrainGrowthThread.h>
#include <boost/thread/barrier.hpp>

const int BaseGrainGrowthProcessor::MAX_CELLS_PER_THREAD = 10000;
const int BaseGrainGrowthProcessor::MAX_THREAD_COUNT = 12;

BaseGrainGrowthProcessor::BaseGrainGrowthProcessor() : Processor(new TwoDimensionalNeighbourhoodService<GrainCell>) {}

bool BaseGrainGrowthProcessor::process(Grid<GrainCell> &grid) {
    auto gridSize = grid.getHeight() * grid.getWidth();
    int threadCount = gridSize / MAX_CELLS_PER_THREAD;
    threadCount = std::min(threadCount, MAX_THREAD_COUNT);
    threadCount = std::max(threadCount, 1);
    int chunkSize = gridSize / threadCount;
    int reminder = gridSize % threadCount;

    std::vector<unsigned short> results;
    results.assign(threadCount, 0);
    boost::barrier barrier(threadCount);
    ThreadVector threads;
    threads.reserve(threadCount);

    for (int i = 0; i < threadCount; ++i) {
        auto currentReminder = i == threadCount - 1 ? reminder : 0;
        threads.emplace_back(GrainGrowthThread(grid, *neighbourhoodTransferObject, barrier), i, chunkSize, currentReminder, &results[i]);
    }

    auto touched = false;
    for (int i = 0; i < threadCount; ++i) {
        threads[i].join();
        if (results[i]) {
            touched = true;
        }
    }

    return touched;
}

void BaseGrainGrowthProcessor::reset() {}

NeighbourhoodTransferObject<GrainCell> *BaseGrainGrowthProcessor::getNeighbourhoodTransferObject() {
    return neighbourhoodTransferObject;
}
