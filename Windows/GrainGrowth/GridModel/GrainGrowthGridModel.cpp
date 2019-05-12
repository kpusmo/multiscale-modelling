#include <cmath>
#include "GrainGrowthGridModel.h"

GrainGrowthGridModel::GrainGrowthGridModel() : timer(new QTimer(this)) {
    connect(timer, SIGNAL(timeout()), SLOT(nextStep()));
}

GrainGrowthGridModel::~GrainGrowthGridModel() {
    delete timer;
}

void GrainGrowthGridModel::nextStep() {
    beginResetModel();
    simulate();
    endResetModel();
}

void GrainGrowthGridModel::drawGrid(unsigned short height, unsigned short width) {
    stopSimulation();
    beginResetModel();
    grid.reset(height, width);
    endResetModel();
}

void GrainGrowthGridModel::stopSimulation() {
    timer->stop();
    isRunning = false;
}

void GrainGrowthGridModel::startSimulation(const BoundaryCondition &bc) {
    if (isRunning) {
        return;
    }
    timer->start(TIMER_INTERVAL);
    isRunning = true;
    grid.setBoundaryCondition(bc);
    previousState = grid;
    beginResetModel();
    simulate();
    endResetModel();
}

void GrainGrowthGridModel::simulate() {
    bool touched = false;
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            if (grid[i][j].getState() != 0) {
                continue;
            }
            touched = true;
            auto mostFrequentNeighbourCell = findMostFrequentNeighbourCell(i, j);
            if (mostFrequentNeighbourCell != nullptr) {
                grid[i][j] = *mostFrequentNeighbourCell;
            }
        }
    }
    if (!touched) {
        stopSimulation();
    }
    previousState = grid;
}

const GrainCell *GrainGrowthGridModel::findMostFrequentNeighbourCell(int i, int j) const {
    std::map<GrainCell, unsigned short> neighbourStateMap;
    neighbourStateMap[previousState[i][j - 1]]++;
    neighbourStateMap[previousState[i][j + 1]]++;
    neighbourStateMap[previousState[i - 1][j]]++;
    neighbourStateMap[previousState[i + 1][j]]++;

    unsigned short maxValue = 0;
    const GrainCell *max = nullptr;
    for (const auto &it : neighbourStateMap) {
        if (it.second > maxValue && it.first.getState() != 0) {
            max = &(it.first);
            maxValue = it.second;
        }
    }
    return max;
}

void GrainGrowthGridModel::onCellSelected(const QModelIndex &index) {
    if (!isCellSelectionAvailable()) {
        return;
    }
    int column = index.column();
    int row = index.row();

    if (grid[row][column].getState() != 0) {
        grid[row][column].reset();
    } else {
        grid[row][column].changeState();
    }
    emit dataChanged(index, index);
}

bool GrainGrowthGridModel::isCellSelectionAvailable() {
    return !isRunning;
}

void GrainGrowthGridModel::setHomogeneousComposition(int rows, int columns) {
    double deltaRows = 1. * grid.getHeight() / rows;
    double deltaColumns = 1. * grid.getWidth() / columns;
    if (deltaRows == 0) {
        deltaRows = 1;
        qDebug() << "Grid is too small for" << rows << "rows." << grid.getWidth() << "rows set.";
    }
    if (deltaColumns == 0) {
        deltaRows = 1;
        qDebug() << "Grid is too small for" << columns << "columns." << grid.getHeight() << "columns set.";
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            grid[floor(i * deltaRows)][floor(j * deltaColumns)].changeState();
        }
    }
}

void GrainGrowthGridModel::setRandomComposition(int count, int radius) {
    if (radius <= 0) {
        grid.changeRandomCellStates(count);
        return;
    }
    auto minDimension = std::min(grid.getWidth(), grid.getHeight());
    if (radius >= minDimension) {
        radius = minDimension - 1;
        qDebug() << "Radius too large; using radius:" << radius;
    }
    bool **map = getGridMapForRandomComposition(radius);
    std::random_device rd;
    std::mt19937 generator(rd());
    for (int counter = 0; counter < count; ++counter) {
        std::vector<std::pair<int, int>> availableCells = getCoordinatesOfAvailableCells(map);
        if (availableCells.empty()) {
            qDebug() << "Too many grains requested. Set" << counter << "grains";
            break;
        }
        //change state of random correct cell
        std::uniform_int_distribution<> distribution(0, static_cast<int>(availableCells.size() - 1));
        auto coordinates = availableCells[distribution(generator)];
        grid[coordinates.first][coordinates.second].changeState();
        //update map - set chosen cell and its neighbours as unavailable
        markCellWithNeighboursAsUnavailable(coordinates.first, coordinates.second, map, radius);
    }
    //delete map
    for (int i = 0; i < grid.getHeight(); ++i) {
        delete[] map[i];
    }
    delete[] map;
}

bool **GrainGrowthGridModel::getGridMapForRandomComposition(int radius) {
    auto map = new bool *[grid.getHeight()];
    for (int i = 0; i < grid.getHeight(); ++i) {
        map[i] = new bool [grid.getWidth()];
    }
    //disable neighbours of non-zero cells
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            map[i][j] = grid[i][j].getState() == 0;
            if (!map[i][j]) {
                markCellWithNeighboursAsUnavailable(i, j, map, radius);
            }
        }
    }
    return map;
}

void GrainGrowthGridModel::markCellWithNeighboursAsUnavailable(int a, int b, bool **map, int radius) {
    for (int i = std::max(0, a - radius); i <= std::min(a + radius, grid.getHeight() - 1); ++i) {
        auto delta = std::abs(i - a);
        for (int j = std::max(0, b - radius + delta); j <= std::min(b + radius - delta, grid.getWidth() - 1); ++j) {
            map[i][j] = false;
        }
    }
}

std::vector<std::pair<int, int>> GrainGrowthGridModel::getCoordinatesOfAvailableCells(bool **map) {
    std::vector<std::pair<int, int>> availableCells;
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            if (map[i][j]) {
                availableCells.emplace_back(i, j);
            }
        }
    }
    return availableCells;
}
