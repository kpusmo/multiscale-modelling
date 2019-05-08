#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-default-arguments"

#include <GridModel.h>
#include <QDebug>
#include <iostream>

GridModel::GridModel(QObject *parent) : QAbstractTableModel(parent) {}

int GridModel::rowCount(const QModelIndex &index) const {
    return grid.getHeight();
}

int GridModel::columnCount(const QModelIndex &index) const {
    return grid.getWidth();
}

QVariant GridModel::data(const QModelIndex &index, int role) const {
    int row = index.row();
    int column = index.column();

    if (role == Qt::BackgroundRole) {
        return grid[row][column].getColor();
    }
    return QVariant();
}

void GridModel::setCellCount(unsigned short cellCount, int highStateCount) {
    beginResetModel();
    grid.reset(1, cellCount);
    grid.setRandomHighStates(highStateCount);
    endResetModel();
}

void GridModel::setSimulationSteps(unsigned short rows) {
    if (grid.getWidth() == 0) {
        return;
    }
    beginResetModel();
    grid.setHeight(static_cast<unsigned short>(rows + 1));
    endResetModel();
}

void GridModel::simulate(unsigned short rule) {
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

void GridModel::onCellSelected(const QModelIndex &index) {
    if (grid.getHeight() != 1) { //selection available only before simulation
        return;
    }
    int column = index.column();
    int row = index.row();

    auto newState = static_cast<unsigned short>((grid[row][column].getState() + 1) % 2);
    grid[row][column].setState(newState);
    emit dataChanged(index, index);
}
