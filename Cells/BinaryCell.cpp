#include "BinaryCell.h"

#include "BinaryCell.h"

BinaryCell::BinaryCell(bool isFake) : fake(isFake) {}

void BinaryCell::setState(unsigned newState) {
    if (newState > 1) {
        throw std::runtime_error("Cell's state can only be either 0 or 1, provided: " + std::to_string(newState));
    }
    state = newState;
}

QColor BinaryCell::getColor() const {
    return colors[state];
}

int BinaryCell::getState() const {
    return state;
}

void BinaryCell::changeState() {
    state = (state + 1) % 2;
}
