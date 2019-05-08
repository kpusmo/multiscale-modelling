#ifndef GRID_H
#define GRID_H

#include "Row.h"
#include <vector>
#include <algorithm>
#include <stdexcept>

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

    void setRandomHighStates(int count);

protected:
    inline short getIndex(short i) const;

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
    return grid[getIndex(i)];
}

template<typename T>
short Grid<T>::getIndex(short i) const {
    if (i < 0 || i >= height) {
        throw std::runtime_error("Grid index out of range");
    }
    return i;
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
void Grid<T>::setRandomHighStates(int count) {
    if (count > width) {
        count = width;
    }
    for (int i = 0; i < count; ++i) {
        auto index = random() % width;
        grid[0][index].setState(1);
    }
}

#endif //GRID_H

