#ifndef WIELOSKALOWE_BASEGRAINGROWTHPROCESSOR_H
#define WIELOSKALOWE_BASEGRAINGROWTHPROCESSOR_H


#include <Cells/GrainCell.h>
#include <AbstractProcessor/Processor.h>

class BaseGrainGrowthProcessor : public Processor<GrainCell> {
public:
    BaseGrainGrowthProcessor();

    bool process(Grid<GrainCell> &grid) override;

    void reset() override;

protected:
    Grid<GrainCell> previousState;

    NeighbourhoodService<GrainCell> *getNeighbourhoodService() override;

private:
    const GrainCell *findMostFrequentNeighbourCell(Grid<GrainCell> &grid, int i, int j);

    void initCellNeighbourMap(Grid<GrainCell> &grid, int i, int j);
};


#endif //WIELOSKALOWE_BASEGRAINGROWTHPROCESSOR_H
