#ifndef ROW_H
#define ROW_H

#include <vector>
#include <cstdlib>
#include "BoundaryCondition.h"
#include <QDebug>

template<typename T>
class Row {
public:
    explicit Row(unsigned short l, const BoundaryCondition &bc = BoundaryCondition::PERIODICAL);

    Row(const Row &toCopy);

    T &operator[](short i);

    const T &operator[](short i) const;

    void setBoundaryCondition(const BoundaryCondition &newBc);

protected:
    BoundaryCondition boundaryCondition{BoundaryCondition::PERIODICAL};
    unsigned short length;
    std::vector<T> row;

    inline short getPeriodicalBoundaryConditionIndex(short i) const;
};

template<typename T>
Row<T>::Row(unsigned short l, const BoundaryCondition &bc) : length(l), boundaryCondition(bc) {
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
    return const_cast<T &>((*const_cast<const Row *>(this))[i]);
}

template<typename T>
const T &Row<T>::operator[](short i) const {
    switch (boundaryCondition) {
        case BoundaryCondition::PERIODICAL:
            return row[getPeriodicalBoundaryConditionIndex(i)];
        case BoundaryCondition::ABSORBING:
            if (i < 0 || i >= length) {
                static auto def = T();
                return def;
            }
            return row[i];
    }
}

template<typename T>
short Row<T>::getPeriodicalBoundaryConditionIndex(short i) const {
    int index = abs(i) % length;
    return static_cast<short>(i >= 0 || index == 0 ? index : length - index);
}

template<typename T>
void Row<T>::setBoundaryCondition(const BoundaryCondition &newBc) {
    boundaryCondition = newBc;
}

#endif //ROW_H
