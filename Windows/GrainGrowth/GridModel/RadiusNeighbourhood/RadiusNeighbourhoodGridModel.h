#ifndef WIELOSKALOWE_RADIUSNEIGHBOURHOODGRIDMODEL_H
#define WIELOSKALOWE_RADIUSNEIGHBOURHOODGRIDMODEL_H


#include "Windows/GrainGrowth/GridModel/Base/GrainGrowthGridModel.h"

class RadiusNeighbourhoodGridModel : public GrainGrowthGridModel {
public:

    void setNeighbourhoodRadius(int radius);

    void startSimulation(const BoundaryCondition &bc) override;

    void simulate() override;
protected:
    int neighbourhoodRadius{};
    CoordinatesVector coordinatesOfCellsToProcess;

    void initCellsToProcess();

    CoordinatesVector updateZeroCellsInRadius(int cellY, int cellX);

    NeighbourhoodService *getNeighbourhoodService() override;
};


#endif //WIELOSKALOWE_RADIUSNEIGHBOURHOODGRIDMODEL_H
