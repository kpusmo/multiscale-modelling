#ifndef WIELOSKALOWE_GRAINCELL_H
#define WIELOSKALOWE_GRAINCELL_H

#include <QColor>
#include <map>
#include <Types/Types.h>
#include <random>
#include "RgbColor.h"

class GrainCell {
public:
    GrainCell();

    explicit GrainCell(bool isFake);

    GrainCell(const GrainCell &other);

    void reset();

    void changeState();

    unsigned getState() const;

    unsigned getPreviousState() const;

    void resetPreviousState();

    QColor getColor() const;

    const RealCoordinates &getCenterOfGravity() const;

    GrainCell &operator=(const GrainCell &other);

    bool operator<(const GrainCell &other) const; //for std::map

    bool operator==(const GrainCell &other) const; //for std::map

    /**
     * @param cellState
     * @return true if map was empty, false otherwise
     */
    bool addStateToMap(int cellState, Coordinates coordinates);

    StateWithCoordinates getMostFrequentNeighbourCoordinates() const;

    void clearNeighbourMap();

    static void resetColorsAndState();

    bool isFake() const;

protected:
    static unsigned nextState;
    static ColorMap usedColors;
    /** From upper left corner */
    RealCoordinates centerOfGravity;
    unsigned state;
    unsigned previousState;
    QColor color;
    NeighbourStateCountMap neighbourStateMap{};
    bool fake{false};

    void setCenterOfGravity();
};

struct StateWithCoordinates {
    int state;
    Coordinates coordinates;

    bool operator<(const StateWithCoordinates &other) const {
        return state < other.state;
    }
};


#endif //WIELOSKALOWE_GRAINCELL_H
