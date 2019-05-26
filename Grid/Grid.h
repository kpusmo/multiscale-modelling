#ifndef GRID_H
#define GRID_H

#include "Row.h"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "BoundaryCondition.h"
#include <QDebug>
#include <random>

template<typename T>
class Grid {
public:
    Grid() = default;

    Grid(unsigned h, unsigned w);

    Row<T> &operator[](int i);

    const Row<T> &operator[](int i) const;

    void reset(unsigned h, unsigned w);

    void addRows(int count);

    unsigned getHeight() const;

    unsigned getWidth() const;

    void changeRandomCellStates(int count);

    void setBoundaryCondition(const BoundaryCondition &newBc);

protected:
    inline int getPeriodicalBoundaryConditionIndex(int i) const;

    BoundaryCondition boundaryCondition{BoundaryCondition::PERIODICAL};
    unsigned height{0};
    unsigned width{0};

    std::vector<Row<T>> grid;
};

template<typename T>
Grid<T>::Grid(unsigned h, unsigned w) : height(h), width(w) {
    grid.assign(h, Row<T>(w));
}

template<typename T>
Row<T> &Grid<T>::operator[](int i) {
    return const_cast<Row<T> &>((*const_cast<const Grid *>(this))[i]);
}

template<typename T>
const Row<T> &Grid<T>::operator[](int i) const {
    switch (boundaryCondition) {
        case BoundaryCondition::PERIODICAL:
            return grid[getPeriodicalBoundaryConditionIndex(i)];
        case BoundaryCondition::ABSORBING:
            if (i < 0 || i >= height) {
                static auto fake = Row<T>(true);
                return fake;
            }
            return grid[i];
    }
}

template<typename T>
int Grid<T>::getPeriodicalBoundaryConditionIndex(int i) const {
    int index = abs(i) % height;
    return (i >= 0 || index == 0) ? index : height - index;
}

template<typename T>
void Grid<T>::reset(unsigned h, unsigned w) {
    height = h;
    width = w;
    grid.clear();
    grid.assign(h, Row<T>(w, boundaryCondition));
}

template<typename T>
unsigned Grid<T>::getHeight() const {
    return height;
}

template<typename T>
unsigned Grid<T>::getWidth() const {
    return width;
}

template<typename T>
void Grid<T>::addRows(int count) {
    grid.insert(grid.begin() + height, static_cast<unsigned long>(count), Row<T>(width, boundaryCondition));
    height += count;
}

template<typename T>
void Grid<T>::changeRandomCellStates(int count) {
    auto size = width * height;
    if (count > size) {
        count = size;
    }
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distributionWidth(0, width - 1);
    std::uniform_int_distribution<> distributionHeight(0, height - 1);
    for (int counter = 0; counter < count; ++counter) {
        auto j = distributionWidth(generator);
        auto i = distributionHeight(generator);
        if (grid[i][j].getState() == 0) {
            grid[i][j].changeState();
        }
    }
}

template<typename T>
void Grid<T>::setBoundaryCondition(const BoundaryCondition &newBc) {
    boundaryCondition = newBc;
    for (auto &row : grid) {
        row.setBoundaryCondition(newBc);
    }
}

#endif //GRID_H

