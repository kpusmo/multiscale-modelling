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

unsigned BinaryCell::getState() const {
    return state;
}

void BinaryCell::changeState() {
    state = (state + 1) % 2;
}

bool BinaryCell::isFake() const {
    return fake;
}

const RealCoordinates &BinaryCell::getCenterOfGravity() const {
    static auto coordinates = RealCoordinates(0, 0);
    return coordinates;
}
