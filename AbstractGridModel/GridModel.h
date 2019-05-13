#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-default-arguments"
#ifndef WIELOSKALOWE_GRIDMODEL_H
#define WIELOSKALOWE_GRIDMODEL_H


#include "BaseGridModel.h"
#include <QDebug>
#include <QSize>
#include "Grid/Grid.h"

template<typename T>
class GridModel : public BaseGridModel {
public:
    int rowCount(const QModelIndex &index = QModelIndex()) const override;

    int columnCount(const QModelIndex &index = QModelIndex()) const override;

    virtual void simulate() = 0;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public slots:
    void onCellSelected(const QModelIndex &index) override;

protected:
    Grid<T> grid;

    virtual bool isCellSelectionAvailable() = 0;
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

template<typename T>
QVariant GridModel<T>::data(const QModelIndex &index, int role) const {
    int row = index.row();
    int column = index.column();

    if (role == Qt::BackgroundRole) {
        return grid[row][column].getColor();
    }
    return QVariant();
}

#endif //WIELOSKALOWE_GRIDMODEL_H

#pragma clang diagnostic pop