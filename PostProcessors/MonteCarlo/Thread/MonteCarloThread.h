#ifndef WIELOSKALOWE_MONTECARLOTHREAD_H
#define WIELOSKALOWE_MONTECARLOTHREAD_H


#include <Grid/Grid.h>
#include <Cells/GrainCell.h>
#include <Neighbourhood/Neighbourhood.h>
#include <Neighbourhood/Service/NeighbourhoodService.h>

class MonteCarloThread {
public:
    MonteCarloThread() = delete;

    MonteCarloThread(Grid<GrainCell> &g, const NeighbourhoodTransferObject<GrainCell> &dto, double ktFactor);

    MonteCarloThread(const MonteCarloThread &other);

    ~MonteCarloThread();

    MonteCarloThread &operator=(const MonteCarloThread &other) = delete;

    void operator()(int threadIndex, int chunkSize, int reminder, CoordinatesCellSet *grainBoundaryCells, CoordinatesCellSet *out);

protected:
    Grid<GrainCell> &grid;
    NeighbourhoodTransferObject<GrainCell> neighbourhoodTransferObject;
    NeighbourhoodService<GrainCell> *neighbourhoodService;
    std::mt19937 randomNumberGenerator;
    double kt;

    int getRandomInteger(int min, int max);

    void seedRandomNumberGenerator();

    NeighbourhoodTransferObject<GrainCell> getNeighbourhoodTransferObject();

    double getRandomRealNumber(double min, double max);

    int calculateFreeEnergy(const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &neighbourCoordinates);

    std::vector<int> getDifferentStateIndices(const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &neighbourCoordinates);

    GrainCell *getRandomNeighbourWithDifferentState(const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &cellNeighbours);

    CoordinatesVector findGrainBoundaryCells(int i, int j);
};


#endif //WIELOSKALOWE_MONTECARLOTHREAD_H
