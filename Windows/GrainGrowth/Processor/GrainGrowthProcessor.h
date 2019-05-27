#ifndef WIELOSKALOWE_GRAINGROWTHPROCESSOR_H
#define WIELOSKALOWE_GRAINGROWTHPROCESSOR_H


#include <Windows/GrainGrowth/Processor/BaseGrainGrowthProcessor.h>

class GrainGrowthProcessor : public BaseGrainGrowthProcessor {
public:
    bool process(Grid<GrainCell> &grid) override;

    GrainGrowthProcessor *setNeighbourhoodRadius(int radius);

    void reset() override;

protected:
    CoordinatesVector coordinatesOfCellsToProcess;
    int neighbourhoodRadius{};

    NeighbourhoodService<GrainCell> *getNeighbourhoodService() override;

private:
    void initCellsToProcess(Grid<GrainCell> &grid);

    CoordinatesVector updateZeroCellsInRadius(Grid<GrainCell> &grid, Coordinates cellCoordinates);
};


#endif //WIELOSKALOWE_GRAINGROWTHPROCESSOR_H
