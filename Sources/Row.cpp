#include "Row.h"

Row::Row(unsigned short l) : length(l) {
    row.assign(l, Cell());
}

Row::Row(const Row &toCopy) {
    length = toCopy.length;
    row.assign(length, Cell());
    for (unsigned short i = 0; i < length; i++) {
        row[i] = toCopy.row[i];
    }
}

Cell &Row::operator[](short i) {
    int index = abs(i) % length;
    if (i < 0) {
        return row[length - index];
    }
    return row[index];
}
