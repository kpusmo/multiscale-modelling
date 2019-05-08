#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-default-arguments"
#ifndef WIELOSKALOWE_GRIDMODEL_H
#define WIELOSKALOWE_GRIDMODEL_H


#include "BaseGridModel.h"
#include <QDebug>
#include "Grid/Grid.h"

template<typename T>
class GridModel : public BaseGridModel {
public:
    int rowCount(const QModelIndex &index = QModelIndex()) const override;

    int columnCount(const QModelIndex &index = QModelIndex()) const override;

    virtual void simulate() = 0;

public slots:
    void onCellSelected(const QModelIndex &index) override;

protected:
    virtual bool isCellSelectionAvailable() = 0;

    Grid<T> grid;
};

template<typename T>
int GridModel<T>::rowCount(const QModelIndex &index) const {
    return grid.getHeight();
}

template<typename T>
int GridModel<T>::columnCount(const QModelIndex &index) const {
    return grid.getWidth();
}

template<typename T>
void GridModel<T>::onCellSelected(const QModelIndex &index) {
    if (!isCellSelectionAvailable()) {
        return;
    }
    int column = index.column();
    int row = index.row();

    grid[row][column].changeState();
    emit dataChanged(index, index);
}

#endif //WIELOSKALOWE_GRIDMODEL_H

#pragma clang diagnostic pop