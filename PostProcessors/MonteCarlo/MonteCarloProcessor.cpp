#include "MonteCarloProcessor.h"
#include <Neighbourhood/Service/TwoDimensionalNeighbourhoodService.h>
#include <PostProcessors/MonteCarlo/Thread/MonteCarloThread.h>

const int MonteCarloProcessor::MAX_CELLS_PER_THREAD = 500;
const int MonteCarloProcessor::MAX_THREAD_COUNT = 12;

MonteCarloProcessor::MonteCarloProcessor() : Processor(new TwoDimensionalNeighbourhoodService<GrainCell>) {}

bool MonteCarloProcessor::process(Grid<GrainCell> &grid) {
    if (grainBoundaryCells.empty()) {
        initGrainBoundaryCells(grid);
    }

    auto cellCount = grainBoundaryCells.size();
    int threadCount = cellCount / MAX_CELLS_PER_THREAD;
    threadCount = std::min(threadCount, MAX_THREAD_COUNT);
    threadCount = std::max(threadCount, 1);
    int chunkSize = cellCount / threadCount;
    int reminder = cellCount % threadCount;

    ThreadVector threads;
    std::vector<CoordinatesCellSet> results;
    threads.reserve(threadCount);
    results.assign(threadCount, {});
    for (int i = 0; i < threadCount; ++i) {
        auto currentReminder = i == threadCount - 1 ? reminder : 0;
        threads.emplace_back(MonteCarloThread(grid, *neighbourhoodTransferObject, kt), i, chunkSize, currentReminder, &grainBoundaryCells, &results[i]);
    }

    CoordinatesCellSet cellsForNextIteration;
    for (int i = 0; i < threadCount; ++i) {
        threads[i].join();
        cellsForNextIteration.merge(results[i]);
    }
    grainBoundaryCells = std::move(cellsForNextIteration);

    return !grainBoundaryCells.empty();
}

void MonteCarloProcessor::initGrainBoundaryCells(Grid<GrainCell> &grid) {
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            findGrainBoundaryCells(grid, i, j);
        }
    }
}

void MonteCarloProcessor::findGrainBoundaryCells(Grid<GrainCell> &grid, int i, int j) {
    auto neighbourCoordinates = neighbourhoodService->getCellNeighbourCoordinates(getNeighbourhoodTransferObject()->omitState(grid[i][j].getState())->setCoordinates(Coordinates(i, j)));
    if (!neighbourCoordinates.empty()) {
        grainBoundaryCells.emplace(&grid[i][j], Coordinates(i, j));
    }
    for (const auto &coordinates : neighbourCoordinates) {
        const auto &cell = grid[coordinates.first][coordinates.second];
        grainBoundaryCells.emplace(&grid[coordinates.first][coordinates.second], coordinates);
    }
}

void MonteCarloProcessor::reset() {
    grainBoundaryCells.clear();
}

NeighbourhoodTransferObject<GrainCell> *MonteCarloProcessor::getNeighbourhoodTransferObject() {
    return neighbourhoodTransferObject->reset()->setMode(NeighbourhoodTransferObjectMode::NON_ZERO_ONLY)->ignoreFakes(true);
}

MonteCarloProcessor *MonteCarloProcessor::setKt(double ktFactor) {
    kt = ktFactor;
    return this;
}

MonteCarloProcessor *MonteCarloProcessor::setNeighbourhoodRadius(int radius) {
    neighbourhoodRadius = radius;
    return this;
}
