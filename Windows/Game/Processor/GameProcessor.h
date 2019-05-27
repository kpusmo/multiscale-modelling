#ifndef WIELOSKALOWE_GAMEPROCESSOR_H
#define WIELOSKALOWE_GAMEPROCESSOR_H

#include <Cells/BinaryCell.h>
#include <AbstractProcessor/Processor.h>

class GameProcessor : public Processor<BinaryCell> {
public:
    GameProcessor();

    bool process(Grid<BinaryCell> &grid) override;

    void reset() override;

protected:
    Grid<BinaryCell> previousState;

    NeighbourhoodService<BinaryCell> *getNeighbourhoodService() override;

    unsigned countLivingSurroundingCells(Grid<BinaryCell> &grid, int a, int b);
};


#endif //WIELOSKALOWE_GAMEPROCESSOR_H
