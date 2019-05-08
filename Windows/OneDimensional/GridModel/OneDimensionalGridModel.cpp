#include "OneDimensionalGridModel.h"
#include <QDebug>
#include <iostream>

void OneDimensionalGridModel::setCellCount(unsigned short cellCount, int highStateCount) {
    beginResetModel();
    grid.reset(1, cellCount);
    grid.setRandomHighStates(highStateCount);
    endResetModel();
}

void OneDimensionalGridModel::setSimulationSteps(unsigned short rows) {
    if (grid.getWidth() == 0) {
        return;
    }
    beginResetModel();
    grid.setHeight(static_cast<unsigned short>(rows + 1));
    endResetModel();
}

void OneDimensionalGridModel::simulate() {
    if (grid.getWidth() == 0 || grid.getHeight() == 0) {
        return;
    }
    for (int stepCounter = 0; stepCounter < grid.getHeight() - 1; ++stepCounter) {
        for (unsigned short cellCounter = 0; cellCounter < grid.getWidth(); ++cellCounter) {
            int cellsMask = grid[stepCounter][cellCounter + 1].getState();
            cellsMask += grid[stepCounter][cellCounter].getState() << 1;
            cellsMask += grid[stepCounter][cellCounter - 1].getState() << 2;
            unsigned short ruleMask = rule >> cellsMask;
            grid[stepCounter + 1][cellCounter].setState(static_cast<unsigned short>(ruleMask & 1));
        }
    }
    QModelIndex topLeft = createIndex(1, 0);
    QModelIndex bottomRight = createIndex(grid.getHeight(), grid.getWidth());
    emit dataChanged(topLeft, bottomRight);
}

bool OneDimensionalGridModel::isCellSelectionAvailable() {
    return grid.getHeight() == 1;
}

QVariant OneDimensionalGridModel::data(const QModelIndex &index, int role) const {
    int row = index.row();
    int column = index.column();

    if (role == Qt::BackgroundRole) {
        return grid[row][column].getColor();
    }
    return QVariant();
}

void OneDimensionalGridModel::setRule(unsigned short r) {
    rule = r;
}
