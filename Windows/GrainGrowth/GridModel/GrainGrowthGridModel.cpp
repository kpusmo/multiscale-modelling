#include <cmath>
#include "GrainGrowthGridModel.h"
#include <sstream>

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
    qDebug() << "simulate";
    bool touched = false;
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            if (grid[i][j].getState() != 0) {
                continue;
            }
            auto mostFrequentNeighbourCell = findMostFrequentNeighbourCell(i, j);
            if (mostFrequentNeighbourCell != nullptr) {
                touched = true;
                grid[i][j] = *mostFrequentNeighbourCell;
            }
        }
    }
    if (!touched) {
        stopSimulation();
    }
    previousState = grid;
}

const GrainCell *GrainGrowthGridModel::findMostFrequentNeighbourCell(int i, int j) {
    initCellNeighbourMap(i, j);
    auto result = grid[i][j].getMostFrequentNeighbour();
    grid[i][j].clearNeighbourMap();
    return result;
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
    std::stringstream msgStream;
    if (deltaRows < 1) {
        deltaRows = 1;
        qDebug() << "Grid is too small for" << rows << "rows." << grid.getWidth() << "rows set.";
        msgStream << "Grid is too small for " << rows << " rows. " << grid.getWidth() << " rows set.\n";
    }
    if (deltaColumns < 1) {
        deltaRows = 1;
        qDebug() << "Grid is too small for" << columns << "columns." << grid.getHeight() << "columns set.";
        msgStream << "Grid is too small for " << columns << " columns. " << grid.getHeight() << " columns set.";
    }
    auto message = msgStream.str();
    if (message.length() > 0) {
        emit showMessageBox(QString::fromStdString(message));
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
        std::stringstream msgStream;
        qDebug() << "Radius too large; using radius:" << radius;
        msgStream << "Radius too large; using radius: " << radius;
        emit showMessageBox(QString::fromStdString(msgStream.str()));
    }
    bool **map = getGridMapForRandomComposition(radius);
    std::random_device rd;
    std::mt19937 generator(rd());
    for (int i = 0; i < count; ++i) {
        auto availableCellsCoordinates = getCoordinatesOfAvailableCells(map);
        if (availableCellsCoordinates.empty()) {
            std::stringstream msgStream;
            qDebug() << "Too many grains requested. Set" << i << "grains";
            msgStream << "Too many grains requested. Set " << i << " grains";
            emit showMessageBox(QString::fromStdString(msgStream.str()));
            break;
        }
        //change state of random correct cell
        std::uniform_int_distribution<> distribution(0, static_cast<int>(availableCellsCoordinates.size() - 1));
        auto coordinates = availableCellsCoordinates[distribution(generator)];
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
        map[i] = new bool[grid.getWidth()];
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

void GrainGrowthGridModel::setNeighbourhood(const Neighbourhood &newNeighbourhood) {
    neighbourhood = newNeighbourhood;
}

void GrainGrowthGridModel::initCellNeighbourMap(int i, int j) {
    auto localNeighbourhood = getLocalNeighbourhood();
    switch (localNeighbourhood) {
        case Neighbourhood::VON_NEUMNANN:
            grid[i][j].addNeighbourToMap(previousState[i][j - 1]);
            grid[i][j].addNeighbourToMap(previousState[i][j + 1]);
            grid[i][j].addNeighbourToMap(previousState[i - 1][j]);
            grid[i][j].addNeighbourToMap(previousState[i + 1][j]);
            break;
        case Neighbourhood::MOORE:
            for (int a = i - 1; a < i + 2; ++a) {
                for (int b = j - 1; b < j + 2; ++b) {
                    //it tries to add (i, j) to map, but its state is zero, so it's ignored
                    grid[i][j].addNeighbourToMap(previousState[a][b]);
                }
            }
            break;
        case Neighbourhood::HEXAGONAL_LEFT_TOP:
            grid[i][j].addNeighbourToMap(previousState[i - 1][j - 1]);
            grid[i][j].addNeighbourToMap(previousState[i - 1][j]);
            grid[i][j].addNeighbourToMap(previousState[i][j - 1]);

            grid[i][j].addNeighbourToMap(previousState[i][j + 1]);
            grid[i][j].addNeighbourToMap(previousState[i + 1][j + 1]);
            grid[i][j].addNeighbourToMap(previousState[i + 1][j]);
            break;
        case Neighbourhood::HEXAGONAL_RIGHT_TOP:
            grid[i][j].addNeighbourToMap(previousState[i - 1][j]);
            grid[i][j].addNeighbourToMap(previousState[i - 1][j + 1]);
            grid[i][j].addNeighbourToMap(previousState[i][j + 1]);

            grid[i][j].addNeighbourToMap(previousState[i][j - 1]);
            grid[i][j].addNeighbourToMap(previousState[i + 1][j - 1]);
            grid[i][j].addNeighbourToMap(previousState[i + 1][j]);
            break;
        case Neighbourhood::PENTAGONAL_TOP:
            grid[i][j].addNeighbourToMap(previousState[i][j - 1]);
            grid[i][j].addNeighbourToMap(previousState[i][j + 1]);
            grid[i][j].addNeighbourToMap(previousState[i - 1][j - 1]);
            grid[i][j].addNeighbourToMap(previousState[i - 1][j]);
            grid[i][j].addNeighbourToMap(previousState[i - 1][j + 1]);
            break;
        case Neighbourhood::PENTAGONAL_RIGHT:
            grid[i][j].addNeighbourToMap(previousState[i - 1][j]);
            grid[i][j].addNeighbourToMap(previousState[i + 1][j]);
            grid[i][j].addNeighbourToMap(previousState[i - 1][j + 1]);
            grid[i][j].addNeighbourToMap(previousState[i][j + 1]);
            grid[i][j].addNeighbourToMap(previousState[i + 1][j + 1]);
            break;
        case Neighbourhood::PENTAGONAL_BOTTOM:
            grid[i][j].addNeighbourToMap(previousState[i][j - 1]);
            grid[i][j].addNeighbourToMap(previousState[i][j + 1]);
            grid[i][j].addNeighbourToMap(previousState[i + 1][j - 1]);
            grid[i][j].addNeighbourToMap(previousState[i + 1][j]);
            grid[i][j].addNeighbourToMap(previousState[i + 1][j + 1]);
            break;
        case Neighbourhood::PENTAGONAL_LEFT:
            grid[i][j].addNeighbourToMap(previousState[i - 1][j]);
            grid[i][j].addNeighbourToMap(previousState[i + 1][j]);
            grid[i][j].addNeighbourToMap(previousState[i - 1][j - 1]);
            grid[i][j].addNeighbourToMap(previousState[i][j - 1]);
            grid[i][j].addNeighbourToMap(previousState[i + 1][j - 1]);
            break;
    }
}

Neighbourhood GrainGrowthGridModel::getLocalNeighbourhood() {
    auto localNeighbourhood = neighbourhood;
    if (localNeighbourhood == Neighbourhood::PENTAGONAL_RANDOM) {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> distribution(0, 3);
        localNeighbourhood = PENTAGONALS[distribution(generator)];
    } else if (localNeighbourhood == Neighbourhood::HEXAGONAL_RANDOM) {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> distribution(0, 1);
        localNeighbourhood = HEXAGONALS[distribution(generator)];
    }
    return localNeighbourhood;
}
