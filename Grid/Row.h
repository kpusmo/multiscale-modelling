#ifndef ROW_H
#define ROW_H

#include <vector>
#include <cstdlib>
#include "BoundaryCondition.h"
#include <QDebug>

template<typename T>
class Row {
public:
    explicit Row(unsigned short l);

    Row(const Row &toCopy);

    T &operator[](short i);

    T operator[](short i) const;

    void setBoundaryCondition(const BoundaryCondition &newBc);

protected:
    BoundaryCondition boundaryCondition{BoundaryCondition::PERIODICAL};

    inline short getIndex(short i) const;

    unsigned short length;
    std::vector<T> row;
};

template<typename T>
Row<T>::Row(unsigned short l) : length(l) {
    row.assign(l, T());
}

template<typename T>
Row<T>::Row(const Row &toCopy) {
    length = toCopy.length;
    boundaryCondition = toCopy.boundaryCondition;
    row.assign(length, T());
    for (unsigned short i = 0; i < length; i++) {
        row[i] = toCopy.row[i];
    }
}

template<typename T>
T &Row<T>::operator[](short i) {
    return row[getIndex(i)];
}

template<typename T>
T Row<T>::operator[](short i) const {
    switch (boundaryCondition) {
        case BoundaryCondition::PERIODICAL:
            return row[getIndex(i)];
        case BoundaryCondition::ABSORBING:
            if (i < 0 || i >= length) {
                static auto def = T();
                return def;
            }
            return row[i];
    }
}

template<typename T>
short Row<T>::getIndex(short i) const {
    int index = abs(i) % length;
    return static_cast<short>(i >= 0 || index == 0 ? index : length - index);
}

template<typename T>
void Row<T>::setBoundaryCondition(const BoundaryCondition &newBc) {
    boundaryCondition = newBc;
}

#endif //ROW_H
