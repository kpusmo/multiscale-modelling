#include <Neighbourhood/Service/TwoDimensionalNeighbourhoodService.h>
#include "MonteCarloThread.h"

MonteCarloThread::MonteCarloThread(Grid<GrainCell> &g, const NeighbourhoodTransferObject<GrainCell> &dto, double ktFactor)
        : grid(g), neighbourhoodTransferObject(dto), kt(ktFactor), neighbourhoodService(new TwoDimensionalNeighbourhoodService<GrainCell>) {
    seedRandomNumberGenerator();
}

MonteCarloThread::MonteCarloThread(const MonteCarloThread &other)
        : grid(other.grid), neighbourhoodTransferObject(other.neighbourhoodTransferObject), kt(other.kt), neighbourhoodService(new TwoDimensionalNeighbourhoodService<GrainCell>) {
    seedRandomNumberGenerator();
}

MonteCarloThread::~MonteCarloThread() {
    delete neighbourhoodService;
}

void MonteCarloThread::operator()(int threadIndex, int chunkSize, int reminder, CoordinatesCellSet *grainBoundaryCells, CoordinatesCellSet *out) {
    auto firstElement = threadIndex * chunkSize;
    auto begin = grainBoundaryCells->begin();
    std::advance(begin, firstElement);
    auto end = grainBoundaryCells->begin();
    std::advance(end, firstElement + chunkSize + reminder);
    CoordinatesCellSet cellsToProcess;
    cellsToProcess.insert(begin, end);

    while (!cellsToProcess.empty()) {
        auto index = getRandomInteger(0, static_cast<int>(cellsToProcess.size() - 1));
        auto cellWithCoordinates = cellsToProcess.begin();
        std::advance(cellWithCoordinates, index);
        auto cellNeighbourCoordinates = neighbourhoodService->getCellNeighbourCoordinates(getNeighbourhoodTransferObject().setCoordinates(cellWithCoordinates->second));

        auto initEnergy = calculateFreeEnergy(*cellWithCoordinates, cellNeighbourCoordinates);
        if (initEnergy == 0) {
            cellsToProcess.erase(cellWithCoordinates);
            continue;
        }

        auto randomNeighbour = getRandomNeighbourWithDifferentState(*cellWithCoordinates, cellNeighbourCoordinates);
        if (randomNeighbour == nullptr) {
            cellsToProcess.erase(cellWithCoordinates);
            continue;
        }

        auto fakeNewStateCellWithCoordinates = CellCoordinatesPair(randomNeighbour, cellWithCoordinates->second);
        auto newEnergy = calculateFreeEnergy(fakeNewStateCellWithCoordinates, cellNeighbourCoordinates);

        auto dE = newEnergy - initEnergy;
        auto probability = exp(-dE / kt);
        auto shot = getRandomRealNumber(0., 1.);
        if (shot <= probability) {
            *(cellWithCoordinates->first) = *randomNeighbour;
            auto coordinatesForNextIteration = findGrainBoundaryCells(cellWithCoordinates->second.first, cellWithCoordinates->second.second);
            for (const auto &coordinates : coordinatesForNextIteration) {
                out->emplace(&grid[coordinates.first][coordinates.second], coordinates);
            }
        }

        cellsToProcess.erase(cellWithCoordinates);
    }
}

int MonteCarloThread::getRandomInteger(int min, int max) {
    return std::uniform_int_distribution<>{min, max}(randomNumberGenerator);
}

double MonteCarloThread::getRandomRealNumber(double min, double max) {
    return std::uniform_real_distribution<>{min, max}(randomNumberGenerator);
}

void MonteCarloThread::seedRandomNumberGenerator() {
    std::random_device randomDevice;
    randomNumberGenerator.seed(randomDevice());
}


int MonteCarloThread::calculateFreeEnergy(const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &neighbourCoordinates) {
    auto indices = getDifferentStateIndices(cellWithCoordinates, neighbourCoordinates);
    return static_cast<int>(indices.size());
}

std::vector<int> MonteCarloThread::getDifferentStateIndices(const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &neighbourCoordinates) {
    std::vector<int> indices;
    for (int i = 0; i < neighbourCoordinates.size(); ++i) {
        const auto &neighbour = grid[neighbourCoordinates[i].first][neighbourCoordinates[i].second];
        if (neighbour.getState() != cellWithCoordinates.first->getState()) {
            indices.push_back(i);
        }
    }
    return indices;
}

GrainCell *MonteCarloThread::getRandomNeighbourWithDifferentState(const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &cellNeighbours) {
    auto indices = getDifferentStateIndices(cellWithCoordinates, cellNeighbours);
    if (indices.empty()) {
        return nullptr;
    }
    auto chosenIndex = getRandomInteger(0, static_cast<int>(indices.size() - 1));
    auto coordinates = cellNeighbours[indices[chosenIndex]];
    return &grid[coordinates.first][coordinates.second];
}

CoordinatesVector MonteCarloThread::findGrainBoundaryCells(int i, int j) {
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

NeighbourhoodTransferObject<GrainCell> MonteCarloThread::getNeighbourhoodTransferObject() {
    return *neighbourhoodTransferObject.reset()->setMode(NeighbourhoodTransferObjectMode::NON_ZERO_ONLY)->ignoreFakes(true);
}
