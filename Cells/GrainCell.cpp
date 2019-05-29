#include "GrainCell.h"
#include <random>
#include <QDebug>
#include <cstdlib>

unsigned GrainCell::nextState = 1;
ColorMap GrainCell::usedColors = ColorMap();

GrainCell::GrainCell() : state(0), previousState(0), color(QColor(255, 255, 255)) {
    setCenterOfGravity();
}

GrainCell::GrainCell(bool isFake) : fake(isFake), state(0), previousState(0), color(QColor(255, 255, 255)) {}

GrainCell::GrainCell(const GrainCell &other) {
    state = other.state;
    previousState = other.previousState;
    color = QColor(other.color);
    setCenterOfGravity();
}

void GrainCell::setCenterOfGravity() {
    std::uniform_real_distribution<> distribution(0, 1);
    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator(randomDevice());
    centerOfGravity.first = distribution(randomNumberGenerator);
    centerOfGravity.second = distribution(randomNumberGenerator);
}

void GrainCell::changeState() {
    state = nextState++;
    previousState = state;

    std::uniform_int_distribution<> distribution(0, 255);
    RgbColor rgbColor{};
    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator(randomDevice());
    do {
        rgbColor.r = distribution(randomNumberGenerator);
        rgbColor.g = distribution(randomNumberGenerator);
        rgbColor.b = distribution(randomNumberGenerator);
    } while (usedColors.count(rgbColor) > 0);
    usedColors[rgbColor] = true;
    color.setRgb(rgbColor.r, rgbColor.g, rgbColor.b);
}

unsigned GrainCell::getState() const {
    return state;
}

unsigned GrainCell::getPreviousState() const {
    return previousState;
}

void GrainCell::resetPreviousState() {
    previousState = state;
}

QColor GrainCell::getColor() const {
    return color;
}

GrainCell &GrainCell::operator=(const GrainCell &other) {
    previousState = state;
    state = other.state;
    color = QColor(other.color);
    return *this;
}

bool GrainCell::operator<(const GrainCell &other) const {
    return state < other.state;
}

bool GrainCell::operator==(const GrainCell &other) const {
    return state == other.state;
}

void GrainCell::reset() {
    state = 0;
    color.setRgb(255, 255, 255);
}

const RealCoordinates &GrainCell::getCenterOfGravity() const {
    return centerOfGravity;
}

bool GrainCell::addStateToMap(int cellState, Coordinates coordinates) {
    auto isEmpty = neighbourStateMap.empty();
    if (cellState != 0) {
        auto key = StateWithCoordinates{cellState, coordinates};
        neighbourStateMap[key]++;
    }
    return isEmpty;
}

StateWithCoordinates GrainCell::getMostFrequentNeighbourCoordinates() const {
    unsigned maxValue = 0;
    auto max = StateWithCoordinates{-1};
    for (const auto &mapItem : neighbourStateMap) {
        if (mapItem.second > maxValue) {
            max = mapItem.first;
            maxValue = mapItem.second;
        }
    }
    return max;
}

void GrainCell::clearNeighbourMap() {
    neighbourStateMap.clear();
}

void GrainCell::resetColorsAndState() {
    usedColors.clear();
    nextState = 1;
}

bool GrainCell::isFake() const {
    return fake;
}
