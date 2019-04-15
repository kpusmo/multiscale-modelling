#ifndef ROW_H
#define ROW_H

#include <vector>
#include "Cell.h"

class Row {
public:
    explicit Row(unsigned short l);

    Row(const Row &toCopy);

    Cell &operator[](short i);

    Cell operator[](short i) const;

protected:
    inline short getIndex(short i) const;

    unsigned short length;
    std::vector<Cell> row;
};


#endif //ROW_H
