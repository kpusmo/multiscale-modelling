#ifndef WIELOSKALOWE_RADIUSGRAINGROWTHTHREAD_H
#define WIELOSKALOWE_RADIUSGRAINGROWTHTHREAD_H


#include <Grid/Grid.h>
#include <Cells/GrainCell.h>
#include <Neighbourhood/Neighbourhood.h>
#include <Neighbourhood/Service/NeighbourhoodService.h>
#include <mutex>

class RadiusGrainGrowthThread {
public:
    RadiusGrainGrowthThread() = delete;

    RadiusGrainGrowthThread(Grid<GrainCell> &g, const NeighbourhoodTransferObject<GrainCell> &dto, std::mutex &m);

    RadiusGrainGrowthThread(const RadiusGrainGrowthThread &other);

    ~RadiusGrainGrowthThread();

    RadiusGrainGrowthThread &operator=(const RadiusGrainGrowthThread &other) = delete;

    void operator()(int index, int chunkSize, int reminder, CoordinatesVector *coordinatesOfCellsToProcess, CoordinatesVector *out);

protected:
    Grid<GrainCell> &grid;
    NeighbourhoodService<GrainCell> *neighbourhoodService;
    NeighbourhoodTransferObject<GrainCell> neighbourhoodTransferObject;
    std::mutex &mutex;

    CoordinatesVector updateZeroCellsInRadius(const Coordinates &cellCoordinates);

    NeighbourhoodTransferObject<GrainCell> getNeighbourhoodTransferObject();
};


#endif //WIELOSKALOWE_RADIUSGRAINGROWTHTHREAD_H
