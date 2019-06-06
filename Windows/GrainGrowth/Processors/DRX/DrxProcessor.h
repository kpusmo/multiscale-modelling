#ifndef WIELOSKALOWE_DRXPROCESSOR_H
#define WIELOSKALOWE_DRXPROCESSOR_H


#include <AbstractProcessor/Processor.h>
#include "DrxTransferObject.h"

class DrxProcessor : public Processor<GrainCell> {
public:
    DrxProcessor();

    bool process(Grid<GrainCell> &grid) override;

    void reset() override;

    void setDto(DrxTransferObject &transferObject);

protected:
    NeighbourhoodTransferObject<GrainCell> *getNeighbourhoodTransferObject() override;

private:
    static const int MAX_CELLS_PER_THREAD;
    static const int MAX_THREAD_COUNT;

    DrxTransferObject dto;
    double t{0};
    double dislocationSum{0};
    double previousRo{0};

    void setInitials();
};


#endif //WIELOSKALOWE_DRXPROCESSOR_H
