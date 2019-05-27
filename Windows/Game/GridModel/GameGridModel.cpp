#include <Windows/Game/Processor/GameProcessor.h>
#include "GameGridModel.h"

GameGridModel::GameGridModel() : timer(new QTimer(this)), GridModel(new GameProcessor) {
    connect(timer, SIGNAL(timeout()), SLOT(nextStep()));
    processor->setNeighbourhood(Neighbourhood::VON_NEUMNANN);
}

GameGridModel::~GameGridModel() {
    delete timer;
}

void GameGridModel::drawGrid(unsigned height, unsigned width, const QString &startingComposition) {
    stopSimulation();
    beginResetModel();
    grid.reset(height, width);
    setStartingComposition(startingComposition);
    endResetModel();
}

void GameGridModel::startSimulation() {
    if (isRunning) {
        return;
    }
    isRunning = true;
    timer->start(TIMER_INTERVAL);
    nextStep();
}

bool GameGridModel::isCellSelectionAvailable() {
    return !isRunning;
}

void GameGridModel::nextStep() {
    beginResetModel();
    processor->process(grid);
    endResetModel();
}

void GameGridModel::stopSimulation() {
    processor->reset();
    timer->stop();
    isRunning = false;
}

void GameGridModel::setStartingComposition(const QString &startingComposition) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distributionWidth(0, grid.getWidth() - 1);
    std::uniform_int_distribution<> distributionHeight(0, grid.getHeight() - 1);
    auto i = distributionHeight(generator);
    auto j = distributionWidth(generator);
    if (startingComposition == "Niezmienny") {
        grid[i][j].setState(1);
        grid[i + 1][j + 1].setState(1);
        grid[i + 1][j + 2].setState(1);
        grid[i][j + 3].setState(1);
        grid[i - 1][j + 2].setState(1);
        grid[i - 1][j + 1].setState(1);
    } else if (startingComposition == "Glider") {
        grid[i][j].setState(1);
        grid[i][j + 1].setState(1);
        grid[i - 1][j].setState(1);
        grid[i - 1][j - 1].setState(1);
        grid[i - 2][j + 1].setState(1);
    } else if (startingComposition == "Oscylator") {
        grid[i][j].setState(1);
        grid[i + 1][j].setState(1);
        grid[i + 2][j].setState(1);
    } else if (startingComposition == "Losowy") {
        grid.changeRandomCellStates(i * j);
    }
}