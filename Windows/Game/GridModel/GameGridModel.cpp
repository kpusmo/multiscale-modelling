#include "GameGridModel.h"

GameGridModel::GameGridModel() : timer(new QTimer(this)) {
    connect(timer, SIGNAL(timeout()), SLOT(nextStep()));
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
    previousState = grid;
    simulate();
}

void GameGridModel::simulate() {
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            auto livingCells = countLivingSurroundingCells(i, j);
            auto currentState = previousState[i][j].getState();
            if (livingCells > 3 || livingCells < 2) {
                grid[i][j].setState(0);
            } else if (currentState == 0 && livingCells == 3) {
                grid[i][j].setState(1);
            }
        }
    }
    previousState = grid;
}

bool GameGridModel::isCellSelectionAvailable() {
    return !isRunning;
}

unsigned GameGridModel::countLivingSurroundingCells(int a, int b) {
    unsigned count = 0;
    //Moore neighbourhood
    for (int i = a - 1; i < a + 2; ++i) {
        for (int j = b - 1; j < b + 2; ++j) {
            count += previousState[i][j].getState();
        }
    }
    count -= previousState[a][b].getState();
    return count;
}

void GameGridModel::nextStep() {
    beginResetModel();
    simulate();
    endResetModel();
}

void GameGridModel::stopSimulation() {
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