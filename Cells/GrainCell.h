#ifndef WIELOSKALOWE_GRAINCELL_H
#define WIELOSKALOWE_GRAINCELL_H

#include <QColor>
#include <map>
#include "RgbColor.h"

class GrainCell {
protected:
    typedef std::pair<double, double> Coordinates;
    typedef std::map<const GrainCell, unsigned short> GrainCellMap;

public:
    GrainCell();

    void reset();

    void changeState();

    short getState() const;

    QColor getColor() const;

    const Coordinates &getCenterOfGravity() const;

    GrainCell &operator=(const GrainCell &other);

    bool operator<(const GrainCell &other) const; //for std::map

    bool operator==(const GrainCell &other) const; //for std::map

    /**
     * @param cell
     * @return true if map was empty, false otherwise
     */
    bool addNeighbourToMap(const GrainCell &cell);

    const GrainCell *getMostFrequentNeighbour() const;

    void clearNeighbourMap();

protected:
    static unsigned short nextState;
    std::map<RgbColor, bool> usedColors;
    /** From upper left corner */
    std::pair<double, double> centerOfGravity;
    unsigned short state;
    QColor color;
    GrainCellMap neighbourStateMap;
};


#endif //WIELOSKALOWE_GRAINCELL_H
