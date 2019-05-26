#include <cmath>
#include "GrainGrowthGridModel.h"
#include <sstream>

GrainGrowthGridModel::GrainGrowthGridModel() : timer(new QTimer(this)), neighbourhoodService(new NeighbourhoodService), monteCarloProcessor(new MonteCarloProcessor) {
    connect(timer, SIGNAL(timeout()), SLOT(nextStep()));
}

GrainGrowthGridModel::~GrainGrowthGridModel() {
    delete timer;
    delete neighbourhoodService;
    delete monteCarloProcessor;
}

void GrainGrowthGridModel::nextStep() {
    if (!isRunning) {
        return;
    }
    beginResetModel();
    switch (stage) {
        case SIMULATION_STAGE:
            simulate();
            break;
        case MONTE_CARLO_STAGE:
            monteCarloStep();
            break;
    }
    endResetModel();
}

void GrainGrowthGridModel::drawGrid(unsigned height, unsigned width) {
    stopSimulation();
    beginResetModel();
    GrainCell::resetColorsAndState();
    grid.reset(height, width);
    endResetModel();
}

void GrainGrowthGridModel::stopSimulation() {
    mcSimulationStep = 0;
    stage = SIMULATION_STAGE;
    monteCarloProcessor->reset();
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
        simulationEnded();
    }
    previousState = grid;
}

const GrainCell *GrainGrowthGridModel::findMostFrequentNeighbourCell(int i, int j) {
    initCellNeighbourMap(i, j);
    auto coordinates = grid[i][j].getMostFrequentNeighbourCoordinates();
    grid[i][j].clearNeighbourMap();
    const auto &cell = grid[coordinates.coordinates.first][coordinates.coordinates.second];
    if (coordinates.state == -1 || cell.isFake()) {
        return nullptr;
    }
    return &grid[coordinates.coordinates.first][coordinates.coordinates.second];
}

void GrainGrowthGridModel::initCellNeighbourMap(int i, int j) {
    auto neighbourCoordinates = getNeighbourhoodService()->ignoreFakes(true)->setMode(NeighbourhoodService::NON_ZERO_ONLY)->getCellNeighbourCoordinates(grid, i, j);
    for (const auto &coordinates : neighbourCoordinates) {
        grid[i][j].addNeighbourToMap(previousState[coordinates.first][coordinates.second], coordinates);
    }
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
    int gridBoundHeight = grid.getHeight() - 1;
    for (int i = std::max(0, a - radius); i <= std::min(a + radius, gridBoundHeight); ++i) {
        auto delta = std::abs(i - a);
        int gridBoundWidth = grid.getWidth() - 1;
        for (int j = std::max(0, b - radius + delta); j <= std::min(b + radius - delta, gridBoundWidth); ++j) {
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

void GrainGrowthGridModel::monteCarloStep() {
    ++mcSimulationStep;
    auto result = monteCarloProcessor->setNeighbourService(getNeighbourhoodService())->process(grid, monteCarloKTFactor);
    if (!result || mcSimulationStep == mcStepCount) {
        stopSimulation();
    }
}

void GrainGrowthGridModel::simulationEnded() {
    switch (postProcessing) {
        case PostProcessing::MONTE_CARLO:
            stage = SimulationStage::MONTE_CARLO_STAGE;
            break;
        case PostProcessing::NONE:
            stopSimulation();
            break;
    }
}

void GrainGrowthGridModel::setPostProcessing(PostProcessing processing) {
    postProcessing = processing;
}

void GrainGrowthGridModel::setMonteCarloKTFactor(double factor) {
    monteCarloKTFactor = factor;
}

void GrainGrowthGridModel::setMonteCarloStepCount(int count) {
    mcStepCount = count;
}

NeighbourhoodService *GrainGrowthGridModel::getNeighbourhoodService() {
    return neighbourhoodService->reset()->setNeighbourhood(neighbourhood);
}
