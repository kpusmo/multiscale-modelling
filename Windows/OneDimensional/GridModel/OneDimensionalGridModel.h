#ifndef WIELOSKALOWE_ONEDIMENSIONALGRIDMODEL_H
#define WIELOSKALOWE_ONEDIMENSIONALGRIDMODEL_H


#include "AbstractGridModel/GridModel.h"
#include "Cells/BinaryCell.h"

class OneDimensionalGridModel : public GridModel<BinaryCell> {
Q_OBJECT
public:
    void setCellCount(unsigned cellCount, int highStateCount = 10);

    void setSimulationSteps(unsigned simulationSteps);

    void setRule(unsigned rule);

    void simulate() override;

protected:
    bool isCellSelectionAvailable() override;

    unsigned rule;
};


#endif //WIELOSKALOWE_ONEDIMENSIONALGRIDMODEL_H