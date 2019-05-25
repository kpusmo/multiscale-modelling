#include "GrainCell.h"
#include <random>
#include <QDebug>
#include <cstdlib>

unsigned short GrainCell::nextState = 1;

GrainCell::GrainCell() : state(0), color(QColor(255, 255, 255)) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<> distribution(0, 1);
    centerOfGravity.first = distribution(generator);
    centerOfGravity.second = distribution(generator);
}

void GrainCell::changeState() {
    state = nextState++;

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, 255);
    RgbColor rgbColor{};
    do {
        rgbColor.r = distribution(generator);
        rgbColor.g = distribution(generator);
        rgbColor.b = distribution(generator);
    } while (usedColors.count(rgbColor) > 0);
    usedColors[rgbColor] = true;
    color.setRgb(rgbColor.r, rgbColor.g, rgbColor.b);
}

short GrainCell::getState() const {
    return state;
}

QColor GrainCell::getColor() const {
    return color;
}

GrainCell &GrainCell::operator=(const GrainCell &other) {
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

const GrainCell::Coordinates &GrainCell::getCenterOfGravity() const {
    return centerOfGravity;
}

bool GrainCell::addNeighbourToMap(const GrainCell &cell) {
    auto isEmpty = neighbourStateMap.empty();
    if (cell.state != 0) {
        neighbourStateMap[cell]++;
    }
    return isEmpty;
}

const GrainCell *GrainCell::getMostFrequentNeighbour() const {
    unsigned short maxValue = 0;
    const GrainCell *max = nullptr;
    for (const auto &mapItem : neighbourStateMap) {
        if (mapItem.second > maxValue) {
            max = &(mapItem.first);
            maxValue = mapItem.second;
        }
    }
    return max;
}

void GrainCell::clearNeighbourMap() {
    neighbourStateMap.clear();
}

