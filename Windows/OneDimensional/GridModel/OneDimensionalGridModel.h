#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-default-arguments"
#ifndef WIELOSKALOWE_ONEDIMENSIONALGRIDMODEL_H
#define WIELOSKALOWE_ONEDIMENSIONALGRIDMODEL_H


#include "AbstractGridModel/GridModel.h"
#include "Grid/Cell.h"

class OneDimensionalGridModel : public GridModel<Cell> {
Q_OBJECT
public:
    void setCellCount(unsigned short cellCount, int highStateCount = 10);

    void setSimulationSteps(unsigned short rows);

    void setRule(unsigned short rule);

    void simulate() override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
protected:
    bool isCellSelectionAvailable() override;

    unsigned short rule;
};


#endif //WIELOSKALOWE_ONEDIMENSIONALGRIDMODEL_H
#pragma clang diagnostic pop