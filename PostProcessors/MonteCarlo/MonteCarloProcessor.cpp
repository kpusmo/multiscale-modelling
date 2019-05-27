#include "MonteCarloProcessor.h"
#include <Neighbourhood/TwoDimensionalNeighbourhoodService.h>

MonteCarloProcessor::MonteCarloProcessor() : randomNumberGenerator(randomDevice()), Processor(new TwoDimensionalNeighbourhoodService<GrainCell>) {}

bool MonteCarloProcessor::process(Grid<GrainCell> &grid) {
    if (grainBoundaryCells.empty()) {
        initGrainBoundaryCells(grid);
    }
    qDebug() << grainBoundaryCells.size();
    bool touched = false;
    auto cellsToProcess = std::move(grainBoundaryCells);
    while (!cellsToProcess.empty()) {
        auto index = getRandomInteger(0, static_cast<int>(cellsToProcess.size() - 1));
        auto cellWithCoordinates = cellsToProcess.begin();
        std::advance(cellWithCoordinates, index);
        auto cellNeighbourCoordinates = getNeighbourhoodService()->getCellNeighbourCoordinates(grid, cellWithCoordinates->second.first, cellWithCoordinates->second.second);

        auto initEnergy = calculateFreeEnergy(grid, *cellWithCoordinates, cellNeighbourCoordinates);
        if (initEnergy == 0) {
            cellsToProcess.erase(cellWithCoordinates);
            continue;
        }

        auto randomNeighbour = getRandomNeighbourWithDifferentState(grid, *cellWithCoordinates, cellNeighbourCoordinates);
        auto fakeNewStateCellWithCoordinates = CellCoordinatesPair(randomNeighbour, cellWithCoordinates->second);
        auto newEnergy = calculateFreeEnergy(grid, fakeNewStateCellWithCoordinates, cellNeighbourCoordinates);

        auto dE = newEnergy - initEnergy;
        auto probability = exp(-dE / kt);
        auto shot = getRandomRealNumber(0., 1.);
        if (shot <= probability) {
            touched = true;
            *(cellWithCoordinates->first) = *randomNeighbour;
            findGrainBoundaryCells(grid, cellWithCoordinates->second.first, cellWithCoordinates->second.second);
        }

        cellsToProcess.erase(cellWithCoordinates);
    }
    return touched;
}

int MonteCarloProcessor::calculateFreeEnergy(const Grid<GrainCell> &grid, const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &neighbourCoordinates) {
    auto indices = getDifferentStateIndices(grid, cellWithCoordinates, neighbourCoordinates);
    return static_cast<int>(indices.size());
}

GrainCell *MonteCarloProcessor::getRandomNeighbourWithDifferentState(Grid<GrainCell> &grid, const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &neighbourCoordinates) {
    auto indices = getDifferentStateIndices(grid, cellWithCoordinates, neighbourCoordinates);
    auto chosenIndex = getRandomInteger(0, static_cast<int>(indices.size() - 1));
    auto coordinates = neighbourCoordinates[indices[chosenIndex]];
    return &grid[coordinates.first][coordinates.second];
}

void MonteCarloProcessor::initGrainBoundaryCells(Grid<GrainCell> &grid) {
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            findGrainBoundaryCells(grid, i, j);
        }
    }
}

void MonteCarloProcessor::findGrainBoundaryCells(Grid<GrainCell> &grid, int i, int j) {
    auto neighbourCoordinates = getNeighbourhoodService()->omitState(grid[i][j].getState())->getCellNeighbourCoordinates(grid, i, j);
    if (!neighbourCoordinates.empty()) {
        grainBoundaryCells.emplace(&grid[i][j], Coordinates(i, j));
    }
    for (const auto &coordinates : neighbourCoordinates) {
        const auto &cell = grid[coordinates.first][coordinates.second];
        grainBoundaryCells.emplace(&grid[coordinates.first][coordinates.second], coordinates);
    }
}

int MonteCarloProcessor::getRandomInteger(int min, int max) {
    return std::uniform_int_distribution<>{min, max}(randomNumberGenerator);
}

double MonteCarloProcessor::getRandomRealNumber(double min, double max) {
    return std::uniform_real_distribution<>{min, max}(randomNumberGenerator);
}

std::vector<int> MonteCarloProcessor::getDifferentStateIndices(const Grid<GrainCell> &grid, const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &neighbourCoordinates) {
    std::vector<int> indices;
    for (int i = 0; i < neighbourCoordinates.size(); ++i) {
        const auto &neighbour = grid[neighbourCoordinates[i].first][neighbourCoordinates[i].second];
        if (neighbour.getState() != cellWithCoordinates.first->getState()) {
            indices.push_back(i);
        }
    }
    return indices;
}

void MonteCarloProcessor::reset() {
    grainBoundaryCells.clear();
}

NeighbourhoodService<GrainCell> *MonteCarloProcessor::getNeighbourhoodService() {
    return neighbourhoodService->reset()->setNeighbourhood(neighbourhood)->setRadius(neighbourhoodRadius)->setMode(NeighbourhoodService<GrainCell>::NON_ZERO_ONLY)->ignoreFakes(true);
}

MonteCarloProcessor *MonteCarloProcessor::setKt(double ktFactor) {
    kt = ktFactor;
    return this;
}

MonteCarloProcessor *MonteCarloProcessor::setNeighbourhoodRadius(int radius) {
    neighbourhoodRadius = radius;
    return this;
}
