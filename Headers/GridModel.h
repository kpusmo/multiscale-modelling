#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-default-arguments"
#ifndef WIELOSKALOWE_GRIDMODEL_H
#define WIELOSKALOWE_GRIDMODEL_H


#include <QAbstractTableModel>
#include "Grid.h"
#include "Cell.h"

class GridModel : public QAbstractTableModel {
Q_OBJECT
public:
    explicit GridModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &index = QModelIndex()) const override;

    int columnCount(const QModelIndex &index = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setCellCount(unsigned short cellCount, int highStateCount = 10);

    void setSimulationSteps(unsigned short rows);

    void simulate(unsigned short rule);

public slots:
    void onCellSelected(const QModelIndex &index);

protected:
    Grid<Cell> grid;
};


#endif //WIELOSKALOWE_GRIDMODEL_H

#pragma clang diagnostic pop