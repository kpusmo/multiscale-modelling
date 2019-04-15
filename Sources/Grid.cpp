#include "Grid.h"

Grid::Grid(unsigned short h, unsigned short w) : height(h), width(w) {
    grid.assign(h, Row(w));
}

Row &Grid::operator[](short i) {
    int index = abs(i) % height;
    if (i < 0) {
        return grid[height - index];
    }
    return grid[index];
}

void Grid::reset(unsigned short h, unsigned short w) {
    height = h;
    width = w;
    grid.clear();
    grid.assign(h, Row(w));
}
