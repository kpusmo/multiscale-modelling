#ifndef WIELOSKALOWE_ONEDIMENSIONALGRIDMODEL_H
#define WIELOSKALOWE_ONEDIMENSIONALGRIDMODEL_H


#include "AbstractGridModel/GridModel.h"
#include "Cells/BinaryCell.h"

class OneDimensionalGridModel : public GridModel<BinaryCell> {
Q_OBJECT
public:
    void setCellCount(unsigned short cellCount, int highStateCount = 10);

    void setSimulationSteps(unsigned short rows);

    void setRule(unsigned short rule);

    void simulate() override;

protected:
    bool isCellSelectionAvailable() override;

    unsigned short rule;
};


#endif //WIELOSKALOWE_ONEDIMENSIONALGRIDMODEL_H