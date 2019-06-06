#ifndef WIELOSKALOWE_BASEGRAINGROWTHPROCESSOR_H
#define WIELOSKALOWE_BASEGRAINGROWTHPROCESSOR_H


#include <Cells/GrainCell.h>
#include <AbstractProcessor/Processor.h>

/**
 * Processor for common neighbourhoods
 */
class BaseGrainGrowthProcessor : public Processor<GrainCell> {
public:
    BaseGrainGrowthProcessor();

    bool process(Grid<GrainCell> &grid) override;

    void reset() override;

protected:
    NeighbourhoodTransferObject<GrainCell> *getNeighbourhoodTransferObject() override;

private:
    static const int MAX_CELLS_PER_THREAD;
    static const int MAX_THREAD_COUNT;
};


#endif //WIELOSKALOWE_BASEGRAINGROWTHPROCESSOR_H
