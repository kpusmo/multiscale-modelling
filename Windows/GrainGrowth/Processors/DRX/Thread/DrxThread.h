#ifndef WIELOSKALOWE_DRXTHREAD_H
#define WIELOSKALOWE_DRXTHREAD_H


#include <Grid/Grid.h>
#include <Cells/GrainCell.h>
#include <Neighbourhood/DataTransferObject/NeighbourhoodTransferObject.h>
#include <Neighbourhood/Service/NeighbourhoodService.h>
#include <mutex>

class DrxThread {
public:
    DrxThread() = delete;

    DrxThread(Grid<GrainCell> &g, const NeighbourhoodTransferObject<GrainCell> &dto, std::mutex &m);

    DrxThread(const DrxThread &other);

    ~DrxThread();

    DrxThread &operator=(const DrxThread &other) = delete;

    void operator()(int threadIndex, int chunkSize, int reminder, double firstDislocationPack, double randomDislocationPack, double roCritical, double *randomDislocationPackRemained, double probability, double *dislocationSum);

protected:
    Grid<GrainCell> &grid;
    NeighbourhoodTransferObject<GrainCell> neighbourhoodTransferObject;
    NeighbourhoodService<GrainCell> *neighbourhoodService;
    std::mt19937 randomNumberGenerator;
    std::mutex &mutex;

    int getRandomInteger(int min, int max);

    void seedRandomNumberGenerator();

    NeighbourhoodTransferObject<GrainCell> getNeighbourhoodTransferObject();

    double getRandomRealNumber(double min, double max);

    CoordinatesVector findGrainBoundaryCells(int i, int j);

    CoordinatesVector getCellNeighbours(Coordinates coordinates);

    bool isOnGrainBoundary(int i, int j);
};


#endif //WIELOSKALOWE_DRXTHREAD_H
