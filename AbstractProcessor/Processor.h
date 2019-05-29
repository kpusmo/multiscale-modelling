#ifndef WIELOSKALOWE_PROCESSOR_H
#define WIELOSKALOWE_PROCESSOR_H

#include <Neighbourhood/Service/NeighbourhoodService.h>

template<typename T>
class Processor {
public:
    explicit Processor(NeighbourhoodService<T> *service) : neighbourhoodService(service) {}

    virtual ~Processor() {
        delete neighbourhoodService;
        delete neighbourhoodTransferObject;
    }

    virtual bool process(Grid<T> &grid) = 0;

    virtual void reset() = 0;

    Processor *setNeighbourhoodTransferObject(NeighbourhoodTransferObject<T> *dto) {
        neighbourhoodTransferObject = dto;
        return this;
    }

protected:
    NeighbourhoodService<T> *neighbourhoodService;
    NeighbourhoodTransferObject<T> *neighbourhoodTransferObject{nullptr};

    virtual NeighbourhoodTransferObject<T> *getNeighbourhoodTransferObject() = 0;
};

#endif //WIELOSKALOWE_PROCESSOR_H
