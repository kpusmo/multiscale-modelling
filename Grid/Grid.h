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

    Grid(unsigned short h, unsigned short w);

    Row<T> &operator[](short i);

    const Row<T> &operator[](short i) const;

    void reset(unsigned short h, unsigned short w);

    void setHeight(unsigned short h);

    unsigned short getHeight() const;

    unsigned short getWidth() const;

    void changeRandomCellStates(int count);

    void setBoundaryCondition(const BoundaryCondition &newBc);

protected:
    inline short getIndex(short i) const;

    BoundaryCondition boundaryCondition{BoundaryCondition::PERIODICAL};
    unsigned short height{0};
    unsigned short width{0};

    std::vector<Row<T>> grid;
};

template<typename T>
Grid<T>::Grid(unsigned short h, unsigned short w) : height(h), width(w) {
    grid.assign(h, Row<T>(w));
}

template<typename T>
Row<T> &Grid<T>::operator[](short i) {
    return grid[getIndex(i)];
}

template<typename T>
const Row<T> &Grid<T>::operator[](short i) const {
    switch (boundaryCondition) {
        case BoundaryCondition::PERIODICAL:
            return grid[getIndex(i)];
        case BoundaryCondition::ABSORBING:
            if (i < 0 || i >= height) {
                static auto def = Row<T>(1);
                return def;
            }
            return grid[i];
    }
}

template<typename T>
short Grid<T>::getIndex(short i) const {
    int index = abs(i) % height;
    return static_cast<short>(i >= 0 || index == 0 ? index : height - index);
}

template<typename T>
void Grid<T>::reset(unsigned short h, unsigned short w) {
    height = h;
    width = w;
    grid.clear();
    grid.assign(h, Row<T>(w));
}

template<typename T>
unsigned short Grid<T>::getHeight() const {
    return height;
}

template<typename T>
unsigned short Grid<T>::getWidth() const {
    return width;
}

template<typename T>
void Grid<T>::setHeight(unsigned short h) {
    if (height > 1) {
        grid.erase(grid.begin() + 1, grid.end());
    }
    height = h;
    grid.insert(grid.begin() + 1, static_cast<unsigned long>(height - 1), Row<T>(width));
}

template<typename T>
void Grid<T>::changeRandomCellStates(int count) {
    if (count > width) {
        count = width;
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
    for (auto &it : grid) {
        it.setBoundaryCondition(newBc);
    }
}

#endif //GRID_H

