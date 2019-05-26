#ifndef ROW_H
#define ROW_H

#include <vector>
#include <cstdlib>
#include "BoundaryCondition.h"
#include <QDebug>

template<typename T>
class Row {
public:
    explicit Row(unsigned l, const BoundaryCondition &bc = BoundaryCondition::PERIODICAL);

    explicit Row(bool isFake);

    Row(const Row &toCopy);

    T &operator[](int i);

    const T &operator[](int i) const;

    void setBoundaryCondition(const BoundaryCondition &newBc);

protected:
    BoundaryCondition boundaryCondition{BoundaryCondition::PERIODICAL};
    unsigned length;
    std::vector<T> row{};
    bool fake{false};

    inline int getPeriodicalBoundaryConditionIndex(int i) const;
};

template<typename T>
Row<T>::Row(unsigned l, const BoundaryCondition &bc) : length(l), boundaryCondition(bc) {
    row.assign(l, T());
}

template<typename T>
Row<T>::Row(bool isFake) : fake(isFake) {}

template<typename T>
Row<T>::Row(const Row &toCopy) {
    length = toCopy.length;
    boundaryCondition = toCopy.boundaryCondition;
    row.assign(length, T());
    for (unsigned i = 0; i < length; i++) {
        row[i] = toCopy.row[i];
    }
}

template<typename T>
T &Row<T>::operator[](int i) {
    return const_cast<T &>((*const_cast<const Row *>(this))[i]);
}

template<typename T>
const T &Row<T>::operator[](int i) const {
    if (fake) {
        static auto def = T(true);
        return def;
    }
    switch (boundaryCondition) {
        case BoundaryCondition::PERIODICAL:
            return row[getPeriodicalBoundaryConditionIndex(i)];
        case BoundaryCondition::ABSORBING:
            if (i < 0 || i >= length) {
                static auto def = T(true);
                return def;
            }
            return row[i];
    }
}

template<typename T>
int Row<T>::getPeriodicalBoundaryConditionIndex(int i) const {
    int index = abs(i) % length;
    return i >= 0 || index == 0 ? index : length - index;
}

template<typename T>
void Row<T>::setBoundaryCondition(const BoundaryCondition &newBc) {
    boundaryCondition = newBc;
}

#endif //ROW_H
