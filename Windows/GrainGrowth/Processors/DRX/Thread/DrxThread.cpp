#include <Neighbourhood/Service/TwoDimensionalNeighbourhoodService.h>
#include "DrxThread.h"
#include <atomic>

DrxThread::DrxThread(Grid<GrainCell> &g, const NeighbourhoodTransferObject<GrainCell> &dto, std::mutex &m) : grid(g), neighbourhoodTransferObject(dto), mutex(m), neighbourhoodService(new TwoDimensionalNeighbourhoodService<GrainCell>) {
    seedRandomNumberGenerator();
}

DrxThread::DrxThread(const DrxThread &other) : grid(other.grid), neighbourhoodTransferObject(other.neighbourhoodTransferObject), mutex(other.mutex), neighbourhoodService(new TwoDimensionalNeighbourhoodService<GrainCell>) {
    seedRandomNumberGenerator();
}

DrxThread::~DrxThread() {
    delete neighbourhoodService;
}

void DrxThread::operator()(int threadIndex, int chunkSize, int reminder, double firstDislocationPack, double randomDislocationPack, double roCritical, double *randomDislocationPackRemained, double probability, double *dislocationSum) {
    auto firstElement = threadIndex * chunkSize;
    auto countOfCellsToProcess = chunkSize + reminder;
    int counter = 0;

    auto width = grid.getWidth();
    auto height = grid.getHeight();

    int i = firstElement / width;
    int j = firstElement % width;

    std::vector<GrainCell *> embryos;
    for (; i < grid.getHeight(); ++i) {
        for (; j < grid.getWidth() && counter < countOfCellsToProcess; ++j, ++counter) {
            //lock for static maxDislocation read/write
            mutex.lock();
            grid[i][j].addDislocationDensity(firstDislocationPack);
            mutex.unlock();
            *dislocationSum += firstDislocationPack;
            if (grid[i][j].getDislocationDensity() >= roCritical && isOnGrainBoundary(i, j)) {
                embryos.push_back(&grid[i][j]);
            }
        }
        j = 0;
    }

    while (*randomDislocationPackRemained > 0) {
        auto randomCellIndex = getRandomInteger(0, width * height - 1);
        i = randomCellIndex / width;
        j = randomCellIndex % width;
        auto shot = getRandomRealNumber(0, 1);
        bool addDislocation =  false, boundary = false;
        if (isOnGrainBoundary(i, j)) {
            if (shot < probability) {
                boundary = true;
                addDislocation = true;
            }
        } else {
            if (shot >= probability) {
                addDislocation = true;
            }
        }
        if (!addDislocation) {
            continue;
        }
        mutex.lock();
        if (*randomDislocationPackRemained <= 0) {
            mutex.unlock();
            break;
        }
        auto &cell = grid[i][j];
        cell.addDislocationDensity(randomDislocationPack);
        *randomDislocationPackRemained -= randomDislocationPack;
        *dislocationSum += randomDislocationPack;
        if (boundary && cell.getDislocationDensity() >= roCritical) {
            embryos.push_back(&cell);
        }
        mutex.unlock();
    }

    i = firstElement / grid.getWidth();
    j = firstElement % grid.getWidth();
    counter = 0;

    for (auto &it : embryos) {
        it->recrystallize();
    }

    for (; i < grid.getHeight(); ++i) {
        for (; j < grid.getWidth() && counter < countOfCellsToProcess; ++j, ++counter) {
            auto cellNeighbourCoordinates = neighbourhoodService->getCellNeighbourCoordinates(getNeighbourhoodTransferObject().setCoordinates(Coordinates(i, j)));
            auto isAnyNeighbourRecrystallized = false;
            for (auto &neighbourCoordinates : cellNeighbourCoordinates) {
                const auto &neighbour = grid[neighbourCoordinates.first][neighbourCoordinates.second];
                if (neighbour.isRecrystallized()) {
                    isAnyNeighbourRecrystallized = true;
                    break;
                }
            }
            if (!isAnyNeighbourRecrystallized) {
                continue;
            }
            auto hasCellHighestDislocation = true;
            for (auto &neighbourCoordinates : cellNeighbourCoordinates) {
                const auto &neighbour = grid[neighbourCoordinates.first][neighbourCoordinates.second];
                if (neighbour.getDislocationDensity() > grid[i][j].getDislocationDensity()) {
                    hasCellHighestDislocation = false;
                    break;
                }
            }
            if (!hasCellHighestDislocation) {
                continue;
            }
            grid[i][j].recrystallize();
        }
        j = 0;
    }
}

bool DrxThread::isOnGrainBoundary(int i, int j) {
    auto neighbourCellCoordinates = neighbourhoodService->getCellNeighbourCoordinates(getNeighbourhoodTransferObject().setCoordinates(Coordinates(i, j))->omitState(grid[i][j].getState()));
    return !neighbourCellCoordinates.empty();
}

CoordinatesVector DrxThread::findGrainBoundaryCells(int i, int j) {
    CoordinatesVector results;
    auto neighbourCoordinates = neighbourhoodService->getCellNeighbourCoordinates(getNeighbourhoodTransferObject().setCoordinates(Coordinates(i, j))->omitState(grid[i][j].getState()));
    if (!neighbourCoordinates.empty()) {
        results.emplace_back(i, j);
    }
    for (const auto &coordinates : neighbourCoordinates) {
        results.push_back(coordinates);
    }
    return results;
}

void DrxThread::seedRandomNumberGenerator() {
    std::random_device randomDevice;
    randomNumberGenerator.seed(randomDevice());
}

int DrxThread::getRandomInteger(int min, int max) {
    return std::uniform_int_distribution<>{min, max}(randomNumberGenerator);
}

double DrxThread::getRandomRealNumber(double min, double max) {
    return std::uniform_real_distribution<>{min, max}(randomNumberGenerator);
}

NeighbourhoodTransferObject <GrainCell> DrxThread::getNeighbourhoodTransferObject() {
    return *neighbourhoodTransferObject.reset();
}
