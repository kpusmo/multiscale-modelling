#include "GrainCell.h"
#include <random>
#include <QDebug>
#include <cstdlib>

unsigned short GrainCell::nextState = 1;

GrainCell::GrainCell() : state(0), color(QColor(255, 255, 255)) {}

void GrainCell::changeState() {
    state = nextState++;
    color = QColor();

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, 360);
    auto h = distribution(generator) * (360 / PALETTE_COLOR_COUNT) % 360;
    color.setHsv(h, 100, 100);
}

short GrainCell::getState() const {
    return state;
}

QColor GrainCell::getColor() const {
    return color;
}

GrainCell &GrainCell::operator=(const GrainCell &other) {
    state = other.state;
    color = QColor(other.color.red(), other.color.green(), other.color.blue());
    return *this;
}

bool GrainCell::operator<(const GrainCell &other) const {
    return state < other.state;
}

void GrainCell::reset() {
    state = 0;
    color.setRgb(255, 255, 255);
}

