#ifndef WIELOSKALOWE_GRAINGROWTHTHREAD_H
#define WIELOSKALOWE_GRAINGROWTHTHREAD_H


#include <Grid/Grid.h>
#include <Cells/GrainCell.h>
#include <Neighbourhood/Service/NeighbourhoodService.h>
#include <boost/thread/barrier.hpp>

class GrainGrowthThread {
public:
    GrainGrowthThread() = delete;

    GrainGrowthThread(Grid<GrainCell> &g, const NeighbourhoodTransferObject<GrainCell> &dto, boost::barrier &b);

    GrainGrowthThread(const GrainGrowthThread &other);

    ~GrainGrowthThread();

    GrainGrowthThread &operator=(const GrainGrowthThread &other) = delete;

    void operator()(int index, int chunkSize, int reminder, unsigned short *touched);

protected:
    Grid<GrainCell> &grid;
    NeighbourhoodTransferObject<GrainCell> neighbourhoodTransferObject;
    boost::barrier &barrier;
    NeighbourhoodService<GrainCell> *neighbourhoodService;

    const GrainCell *findMostFrequentNeighbourCell(int i, int j);

    void initCellNeighbourMap(int i, int j);

    NeighbourhoodTransferObject<GrainCell> getNeighbourhoodTransferObject();
};


#endif //WIELOSKALOWE_GRAINGROWTHTHREAD_H
