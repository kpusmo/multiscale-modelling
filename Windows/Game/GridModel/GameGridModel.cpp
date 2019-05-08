#include "GameGridModel.h"

GameGridModel::GameGridModel() : timer(new QTimer(this)) {
    connect(timer, SIGNAL(timeout()), SLOT(incrementResultIndex()));
}

GameGridModel::~GameGridModel() {
    delete timer;
}

void GameGridModel::drawGrid(unsigned short height, unsigned short width, const QString &startingComposition) {
    timer->stop();
    results.clear();
    currentResult = -1;
    beginResetModel();
    grid.reset(height, width);
    setStartingComposition(startingComposition);
    endResetModel();
}

void GameGridModel::setStartingComposition(const QString &startingComposition) {
//    srand(time(NULL));
//    for (int k = 0; k < 200; ++k) {
    auto i = static_cast<int>(random() % grid.getHeight());
    auto j = static_cast<int>(random() % grid.getWidth());
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
        grid.setRandomHighStates(i * j);
    }
//    }
}

void GameGridModel::startSimulation() {
    results.clear();
    results.push_back(grid);
    currentResult = 0;
    startTimer();
    simulate();
}

void GameGridModel::simulate() {
    qDebug() << "simulate";
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            auto livingCells = countLivingSurroundingCells(i, j);
            auto currentState = results[currentResult][i][j].getState();
            if (livingCells > 3 || livingCells < 2) {
                grid[i][j].setState(0);
            } else if (currentState == 0 && livingCells == 3) {
                grid[i][j].setState(1);
            }
        }
    }
    results.push_back(grid);
}

QVariant GameGridModel::data(const QModelIndex &index, int role) const {
    int row = index.row();
    int column = index.column();

    if (role == Qt::BackgroundRole) {
        if (currentResult > -1) {
            return results[currentResult][row][column].getColor();
        }
        return grid[row][column].getColor();
    }
    return QVariant();
}

bool GameGridModel::isCellSelectionAvailable() {
    return currentResult == -1;
}

unsigned GameGridModel::countLivingSurroundingCells(int a, int b) {
    unsigned count = 0;

    for (int i = a - 1; i < a + 2; ++i) {
        for (int j = b - 1; j < b + 2; ++j) {
            count += results[currentResult][i][j].getState();
        }
    }
    count -= results[currentResult][a][b].getState();
    return count;
}

void GameGridModel::startTimer() {
    timer->start(TIMER_INTERVAL);
}

void GameGridModel::incrementResultIndex() {
    beginResetModel();
    ++currentResult;
    if (currentResult == 2) {
        results.erase(results.begin());
        --currentResult;
    }
    simulate();
    endResetModel();
}
