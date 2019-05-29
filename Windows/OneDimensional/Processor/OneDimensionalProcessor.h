#ifndef WIELOSKALOWE_ONEDIMENSIONALPROCESSOR_H
#define WIELOSKALOWE_ONEDIMENSIONALPROCESSOR_H

#include <Cells/BinaryCell.h>
#include <AbstractProcessor/Processor.h>

class OneDimensionalProcessor : public Processor<BinaryCell> {
public:
    OneDimensionalProcessor();

    bool process(Grid<BinaryCell> &grid) override;

    void reset() override;

    OneDimensionalProcessor *setRule(unsigned r);

protected:
    NeighbourhoodTransferObject<BinaryCell> *getNeighbourhoodTransferObject() override;
    unsigned rule{};
};


#endif //WIELOSKALOWE_ONEDIMENSIONALPROCESSOR_H
