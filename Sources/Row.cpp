#include <Row.h>
#include <iostream>

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
    return row[getIndex(i)];
}

Cell Row::operator[](short i) const {
    return row[getIndex(i)];
}

short Row::getIndex(short i) const {
    int index = abs(i) % length;
    return static_cast<short>(i >= 0 || index == 0 ? index : length - index);
}
