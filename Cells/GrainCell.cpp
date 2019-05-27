#include "GrainCell.h"
#include <random>
#include <QDebug>
#include <cstdlib>

unsigned GrainCell::nextState = 1;
ColorMap GrainCell::usedColors = ColorMap();

GrainCell::GrainCell() : state(0), color(QColor(255, 255, 255)), randomNumberGenerator(randomDevice()) {
    setCenterOfGravity();
}

GrainCell::GrainCell(bool isFake) : fake(isFake), state(0), color(QColor(255, 255, 255)), randomNumberGenerator(randomDevice()) {}

GrainCell::GrainCell(const GrainCell &other) : randomNumberGenerator(randomDevice()) {
    state = other.state;
    color = QColor(other.color);
    setCenterOfGravity();
}

void GrainCell::setCenterOfGravity() {
    std::uniform_real_distribution<> distribution(0, 1);
    centerOfGravity.first = distribution(randomNumberGenerator);
    centerOfGravity.second = distribution(randomNumberGenerator);
}

void GrainCell::changeState() {
    state = nextState++;

    std::uniform_int_distribution<> distribution(0, 255);
    RgbColor rgbColor{};
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

QColor GrainCell::getColor() const {
    return color;
}

GrainCell &GrainCell::operator=(const GrainCell &other) {
    state = other.state;
    color = QColor(other.color);
    centerOfGravity = other.centerOfGravity;
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

bool GrainCell::addNeighbourToMap(const GrainCell &cell, Coordinates coordinates) {
    auto isEmpty = neighbourStateMap.empty();
    int cellState = cell.getState();
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

