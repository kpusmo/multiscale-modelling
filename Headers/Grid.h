#ifndef GRID_H
#define GRID_H

#include "Row.h"
#include <vector>

class Grid {
public:
    Grid() = default;

    Grid(unsigned short h, unsigned short w);

    Row &operator[](short i);

    void reset(unsigned short h, unsigned short w);

protected:
    unsigned short height{0};
    unsigned short width{0};
    std::vector<Row> grid;
};

#endif //GRID_H

