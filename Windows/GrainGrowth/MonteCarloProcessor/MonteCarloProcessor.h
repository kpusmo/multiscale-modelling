#ifndef WIELOSKALOWE_MONTECARLOPROCESSOR_H
#define WIELOSKALOWE_MONTECARLOPROCESSOR_H


#include <Grid/Grid.h>
#include <Cells/GrainCell.h>
#include <Neighbourhood/NeighbourhoodService.h>

class MonteCarloProcessor {
public:
    MonteCarloProcessor();

    bool process(Grid<GrainCell> &grid, double kt);

    MonteCarloProcessor *setNeighbourService(NeighbourhoodService *service);

    void reset();

protected:
    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator;
    CoordinatesCellSet grainBoundaryCells;
    NeighbourhoodService *neighbourhoodService;

    void initGrainBoundaryCells(Grid<GrainCell> &grid);

    int getRandomInteger(int min, int max);

    double getRandomRealNumber(double min, double max);

    void findGrainBoundaryCells(Grid<GrainCell> &grid, int i, int j);

    int calculateFreeEnergy(const Grid<GrainCell> &grid, const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &neighbourCoordinates);

    GrainCell *getRandomNeighbourWithDifferentState(Grid<GrainCell> &grid, const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &neighbourCoordinates);

    std::vector<int> getDifferentStateIndices(const Grid<GrainCell> &grid, const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &neighbourCoordinates);
};


#endif //WIELOSKALOWE_MONTECARLOPROCESSOR_H
