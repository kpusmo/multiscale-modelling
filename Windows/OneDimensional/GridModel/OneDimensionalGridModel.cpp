#include "OneDimensionalGridModel.h"
#include <QDebug>
#include <iostream>

void OneDimensionalGridModel::setCellCount(unsigned cellCount, int highStateCount) {
    beginResetModel();
    grid.reset(1, cellCount);
    grid.changeRandomCellStates(highStateCount);
    endResetModel();
}

void OneDimensionalGridModel::setSimulationSteps(unsigned simulationSteps) {
    if (grid.getHeight() != 1) { //can set simulation steps only after drawing first row
        return;
    }
    beginResetModel();
    grid.addRows(simulationSteps);
    endResetModel();
}

void OneDimensionalGridModel::simulate() {
    if (grid.getWidth() == 0 || grid.getHeight() == 0) {
        return;
    }
    for (int stepCounter = 0; stepCounter < grid.getHeight() - 1; ++stepCounter) {
        for (auto i = 0; i < grid.getWidth(); ++i) {
            int cellsMask = grid[stepCounter][i + 1].getState();
            cellsMask += grid[stepCounter][i].getState() << 1;
            cellsMask += grid[stepCounter][i - 1].getState() << 2;
            auto ruleMask = rule >> cellsMask;
            grid[stepCounter + 1][i].setState(ruleMask & 1);
        }
    }
    QModelIndex topLeft = createIndex(1, 0);
    QModelIndex bottomRight = createIndex(grid.getHeight() - 1, grid.getWidth() - 1);
    emit dataChanged(topLeft, bottomRight);
}

bool OneDimensionalGridModel::isCellSelectionAvailable() {
    return grid.getHeight() == 1;
}

void OneDimensionalGridModel::setRule(unsigned r) {
    rule = r;
}
