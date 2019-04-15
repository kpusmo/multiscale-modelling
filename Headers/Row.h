#ifndef ROW_H
#define ROW_H

#include <vector>
#include "Cell.h"

class Row {
public:
    explicit Row(unsigned short l);

    Row(const Row &toCopy);

    Cell &operator[](short i);

protected:
    unsigned short length;
    std::vector<Cell> row;
};


#endif //ROW_H
