#include <Neighbourhood/Service/TwoDimensionalNeighbourhoodService.h>
#include "DrxProcessor.h"
#include <atomic>
#include <PostProcessors/DRX/Thread/DrxThread.h>
#include <numeric>
#include <fstream>

const int DrxProcessor::MAX_CELLS_PER_THREAD = 10000;
const int DrxProcessor::MAX_THREAD_COUNT = 12;

DrxProcessor::DrxProcessor() : Processor(new TwoDimensionalNeighbourhoodService<GrainCell>) {}

bool DrxProcessor::process(Grid<GrainCell> &grid) {
    auto gridSize = grid.getHeight() * grid.getWidth();
    int threadCount = gridSize / MAX_CELLS_PER_THREAD;
    threadCount = std::min(threadCount, MAX_THREAD_COUNT);
    threadCount = std::max(threadCount, 1);
    int chunkSize = gridSize / threadCount;
    int reminder = gridSize % threadCount;

    t += dto.dt;
    double ro = dto.A / dto.B + (1 - dto.A / dto.B) * exp(-dto.B * t);

    double deltaRo = ro - previousRo;
    previousRo = ro;

    ThreadVector threads;
    threads.reserve(threadCount);

    auto avgDislocation = deltaRo / gridSize;
    auto firstDislocationPack = dto.firstPackPercent * avgDislocation;
    double randomDislocationPackSize = (1 - dto.firstPackPercent) * deltaRo;
    double randomDislocationPack = dto.randomDislocationPackFactor * randomDislocationPackSize;
    std::mutex mutex;
    std::vector<double> dislocationSums;
    dislocationSums.assign(threadCount, 0);
    double roCritical = dto.roCritical / gridSize;

    for (int i = 0; i < threadCount; ++i) {
        auto currentReminder = i == threadCount - 1 ? reminder : 0;
        threads.emplace_back(DrxThread(grid, *neighbourhoodTransferObject, mutex), i, chunkSize, currentReminder, firstDislocationPack, randomDislocationPack, roCritical, &randomDislocationPackSize, dto.boundaryProbability, &dislocationSums[i]);
    }

    for (int i = 0; i < threadCount; ++i) {
        threads[i].join();
        dislocationSum += dislocationSums[i];
    }

    qDebug() << dislocationSum;
    std::ofstream out("dislocations", std::ofstream::app);
    out << dislocationSum << '\n';
    out.close();
    out.open("sigmas", std::ofstream::app);
    double sigma = 1.9 * 2.57 * 8 * sqrt(dislocationSum);
    out << sigma << '\n';

    return true;
}

void DrxProcessor::reset() {
    std::ofstream out;
    out.open("dislocations", std::ofstream::out | std::ofstream::trunc);
    out.close();
    out.open("sigmas", std::ofstream::out | std::ofstream::trunc);
    out.close();
    t = 0;
    setInitials();
}

NeighbourhoodTransferObject<GrainCell> *DrxProcessor::getNeighbourhoodTransferObject() {
    return neighbourhoodTransferObject;
}

void DrxProcessor::setDto(DrxTransferObject &transferObject) {
    dto = transferObject;
    setInitials();
}

void DrxProcessor::setInitials() {
    previousRo = dto.A / dto.B + (1 - dto.A / dto.B) * exp(-dto.B * t);
    dislocationSum = previousRo;
}
