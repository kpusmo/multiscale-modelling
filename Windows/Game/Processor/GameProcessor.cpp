#include "GameProcessor.h"
#include <Neighbourhood/Service/TwoDimensionalNeighbourhoodService.h>

GameProcessor::GameProcessor() : Processor(new TwoDimensionalNeighbourhoodService<BinaryCell>) {}

bool GameProcessor::process(Grid<BinaryCell> &grid) {
    if (previousState.getHeight() == 0) {
        previousState = grid;
    }
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            auto livingCells = countLivingSurroundingCells(grid, i, j);
            auto currentState = previousState[i][j].getState();
            if (livingCells > 3 || livingCells < 2) {
                grid[i][j].setState(0);
            } else if (currentState == 0 && livingCells == 3) {
                grid[i][j].setState(1);
            }
        }
    }
    previousState = grid;
    return true;
}

unsigned GameProcessor::countLivingSurroundingCells(Grid<BinaryCell> &grid, int a, int b) {
    unsigned count = 0;
    auto neighbourCoordinates = neighbourhoodService->getCellNeighbourCoordinates(getNeighbourhoodTransferObject()->setCoordinates(Coordinates(a, b)));
    for (const auto &coordinates : neighbourCoordinates) {
        count += previousState[coordinates.first][coordinates.second].getState();
    }
    return count;
}

void GameProcessor::reset() {
    previousState.reset(0, 0);
}

NeighbourhoodTransferObject<BinaryCell> *GameProcessor::getNeighbourhoodTransferObject() {
    return neighbourhoodTransferObject;
}
