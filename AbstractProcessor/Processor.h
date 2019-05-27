#ifndef WIELOSKALOWE_PROCESSOR_H
#define WIELOSKALOWE_PROCESSOR_H

#include <Neighbourhood/NeighbourhoodService.h>

template<typename T>
class Processor {
public:
    explicit Processor(NeighbourhoodService<T> *service) : neighbourhoodService(service) {}

    ~Processor() {
        delete neighbourhoodService;
    }

    virtual bool process(Grid<T> &grid) = 0;

    virtual void reset() = 0;

    Processor *setNeighbourhood(Neighbourhood newNeighbourhood) {
        neighbourhood = newNeighbourhood;
    }

protected:
    NeighbourhoodService<T> *neighbourhoodService;
    Neighbourhood neighbourhood{};

    virtual NeighbourhoodService<T> *getNeighbourhoodService() = 0;
};

#endif //WIELOSKALOWE_PROCESSOR_H
