#include <Grid.h>

Grid::Grid(unsigned short h, unsigned short w) : height(h), width(w) {
    grid.assign(h, Row(w));
}

Row &Grid::operator[](short i) {
    return grid[getIndex(i)];
}

const Row &Grid::operator[](short i) const {
    return grid[getIndex(i)];
}

short Grid::getIndex(short i) const {
    if (i < 0 || i >= height) {
        throw std::runtime_error("Grid index out of range");
    }
    return i;
}

void Grid::reset(unsigned short h, unsigned short w) {
    height = h;
    width = w;
    grid.clear();
    grid.assign(h, Row(w));
}

unsigned short Grid::getHeight() const {
    return height;
}

unsigned short Grid::getWidth() const {
    return width;
}

void Grid::setHeight(unsigned short h) {
    if (height > 1) {
        grid.erase(grid.begin() + 1, grid.end());
    }
    height = h;
    grid.insert(grid.begin() + 1, static_cast<unsigned long>(height - 1), Row(width));
}
