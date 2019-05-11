#include "BinaryCell.h"

#include "BinaryCell.h"

void BinaryCell::setState(unsigned short newState) {
    if (newState > 1) {
        throw std::runtime_error("Cell's state can only be either 0 or 1, provided: " + std::to_string(newState));
    }
    state = newState;
}

QColor BinaryCell::getColor() const {
    return colors[state];
}

short BinaryCell::getState() const {
    return state;
}

void BinaryCell::changeState() {
    state = static_cast<unsigned short>((state + 1) % 2);
}

