#include "Cell.h"

Cell::Cell() : state(0) {}

void Cell::setState(unsigned short newState) {
    if (newState > 1) {
        throw std::runtime_error("Cell's state can only be either 0 or 1, provided: " + std::to_string(newState));
    }
    state = newState;
}

QColor Cell::getColor() const {
    return colors[state];
}
