#ifndef WIELOSKALOWE_GRAINCELL_H
#define WIELOSKALOWE_GRAINCELL_H

#include <QColor>

class GrainCell {
public:
    GrainCell();

    void reset();

    void changeState();

    short getState() const;

    QColor getColor() const;

    GrainCell &operator=(const GrainCell &other);

    bool operator<(const GrainCell &other) const; //for std::map

protected:
    static unsigned short nextState;
    static const unsigned short PALETTE_COLOR_COUNT = 20;
    unsigned short state;
    QColor color;
};


#endif //WIELOSKALOWE_GRAINCELL_H
