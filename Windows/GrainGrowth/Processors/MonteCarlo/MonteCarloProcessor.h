#ifndef WIELOSKALOWE_MONTECARLOPROCESSOR_H
#define WIELOSKALOWE_MONTECARLOPROCESSOR_H


#include <Grid/Grid.h>
#include <Cells/GrainCell.h>
#include <Neighbourhood/Service/TwoDimensionalNeighbourhoodService.h>
#include <AbstractProcessor/Processor.h>

class MonteCarloProcessor : public Processor<GrainCell> {
public:
    MonteCarloProcessor();

    bool process(Grid<GrainCell> &grid) override;

    void reset() override;

    MonteCarloProcessor *setKt(double ktFactor);

protected:
    CoordinatesCellSet grainBoundaryCells;
    double kt{};

    NeighbourhoodTransferObject<GrainCell> *getNeighbourhoodTransferObject() override;

    void initGrainBoundaryCells(Grid<GrainCell> &grid);

    void findGrainBoundaryCells(Grid<GrainCell> &grid, int i, int j);

private:
    static const int MAX_CELLS_PER_THREAD;
    static const int MAX_THREAD_COUNT;};


#endif //WIELOSKALOWE_MONTECARLOPROCESSOR_H
