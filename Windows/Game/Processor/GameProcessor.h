#ifndef WIELOSKALOWE_GAMEPROCESSOR_H
#define WIELOSKALOWE_GAMEPROCESSOR_H

#include <AbstractProcessor/Processor.h>
#include <Cells/BinaryCell.h>

class GameProcessor : public Processor<BinaryCell> {
public:
    GameProcessor();

    bool process(Grid<BinaryCell> &grid) override;

    void reset() override;

protected:
    Grid<BinaryCell> previousState;

    unsigned countLivingSurroundingCells(Grid<BinaryCell> &grid, int a, int b);

    NeighbourhoodTransferObject<BinaryCell> *getNeighbourhoodTransferObject() override;
};


#endif //WIELOSKALOWE_GAMEPROCESSOR_H
