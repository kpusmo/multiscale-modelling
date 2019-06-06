#ifndef WIELOSKALOWE_GRAINGROWTHPROCESSOR_H
#define WIELOSKALOWE_GRAINGROWTHPROCESSOR_H


#include <Windows/GrainGrowth/Processors/GrainGrowth/BaseGrainGrowthProcessor.h>

/**
 * Processor for radius neighbourhood
 */
class GrainGrowthProcessor : public BaseGrainGrowthProcessor {
public:
    bool process(Grid<GrainCell> &grid) override;

    void reset() override;

protected:
    CoordinatesVector coordinatesOfCellsToProcess;

private:
    static const int MAX_CELLS_PER_THREAD;
    static const int MAX_THREAD_COUNT;

    void initCellsToProcess(Grid<GrainCell> &grid);
};


#endif //WIELOSKALOWE_GRAINGROWTHPROCESSOR_H
