#ifndef WIELOSKALOWE_RGBCOLOR_H
#define WIELOSKALOWE_RGBCOLOR_H

struct RgbColor {
    int r, g, b;

    bool operator<(const RgbColor &other) const { //for std::map
        return r < other.r || g < other.g || b > other.b;
    }
};

#endif //WIELOSKALOWE_RGBCOLOR_H
