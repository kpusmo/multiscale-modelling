#ifndef WIELOSKALOWE_RADIUSNEIGHBOURHOODGRIDMODEL_H
#define WIELOSKALOWE_RADIUSNEIGHBOURHOODGRIDMODEL_H


#include "GrainGrowthGridModel.h"

class RadiusNeighbourhoodGridModel : public GrainGrowthGridModel {
public:

    void setNeighbourhoodRadius(int radius);

    void startSimulation(const BoundaryCondition &bc) override;

    void simulate() override;
protected:
    typedef std::vector<Coordinates> CoordinatesVector;

    int neighbourhoodRadius{};
    CoordinatesVector coordinatesOfCellsToProcess;

    void initCellsToProcess();

    CoordinatesVector updateZeroCellsInRadius(int cellY, int cellX);
};


#endif //WIELOSKALOWE_RADIUSNEIGHBOURHOODGRIDMODEL_H
