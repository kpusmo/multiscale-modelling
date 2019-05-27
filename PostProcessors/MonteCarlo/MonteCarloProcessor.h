#ifndef WIELOSKALOWE_MONTECARLOPROCESSOR_H
#define WIELOSKALOWE_MONTECARLOPROCESSOR_H


#include <Grid/Grid.h>
#include <Cells/GrainCell.h>
#include <Neighbourhood/TwoDimensionalNeighbourhoodService.h>
#include <AbstractProcessor/Processor.h>

class MonteCarloProcessor : public Processor<GrainCell> {
public:
    MonteCarloProcessor();

    bool process(Grid<GrainCell> &grid) override;

    void reset();

    MonteCarloProcessor *setKt(double ktFactor);

    MonteCarloProcessor *setNeighbourhoodRadius(int radius);

protected:
    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator;
    CoordinatesCellSet grainBoundaryCells;
    double kt{};
    int neighbourhoodRadius{};

    NeighbourhoodService<GrainCell> *getNeighbourhoodService() override;

    void initGrainBoundaryCells(Grid<GrainCell> &grid);

    int getRandomInteger(int min, int max);

    double getRandomRealNumber(double min, double max);

    void findGrainBoundaryCells(Grid<GrainCell> &grid, int i, int j);

    static int calculateFreeEnergy(const Grid<GrainCell> &grid, const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &neighbourCoordinates);

    GrainCell *getRandomNeighbourWithDifferentState(Grid<GrainCell> &grid, const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &neighbourCoordinates);

    static std::vector<int> getDifferentStateIndices(const Grid<GrainCell> &grid, const CellCoordinatesPair &cellWithCoordinates, const CoordinatesVector &neighbourCoordinates);
};


#endif //WIELOSKALOWE_MONTECARLOPROCESSOR_H
