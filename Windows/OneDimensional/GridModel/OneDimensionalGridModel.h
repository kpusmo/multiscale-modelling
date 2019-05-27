#ifndef WIELOSKALOWE_ONEDIMENSIONALGRIDMODEL_H
#define WIELOSKALOWE_ONEDIMENSIONALGRIDMODEL_H


#include "AbstractGridModel/GridModel.h"
#include "Cells/BinaryCell.h"

class OneDimensionalGridModel : public GridModel<BinaryCell> {
Q_OBJECT
public:
    OneDimensionalGridModel();

    void setCellCount(unsigned cellCount, int highStateCount = 10);

    void setSimulationSteps(unsigned simulationSteps);

    void setRule(unsigned rule);

    void simulate();

protected:
    bool isCellSelectionAvailable() override;
};


#endif //WIELOSKALOWE_ONEDIMENSIONALGRIDMODEL_H