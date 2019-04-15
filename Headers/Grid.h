#ifndef GRID_H
#define GRID_H

#include "Row.h"
#include <vector>

class Grid {
public:
    Grid() = default;

    Grid(unsigned short h, unsigned short w);

    Row &operator[](short i);

    const Row &operator[](short i) const;

    void reset(unsigned short h, unsigned short w);

    void setHeight(unsigned short h);

    unsigned short getHeight() const;

    unsigned short getWidth() const;

protected:
    inline short getIndex(short i) const;

    unsigned short height{0};
    unsigned short width{0};
    std::vector<Row> grid;
};

#endif //GRID_H

