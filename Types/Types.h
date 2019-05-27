#ifndef WIELOSKALOWE_TYPES_H
#define WIELOSKALOWE_TYPES_H

#include <vector>
#include <map>
#include <set>
#include <thread>

class GrainCell;
struct StateWithCoordinates;
struct RgbColor;

typedef std::pair<double, double> RealCoordinates;

typedef std::pair<int, int> Coordinates;
typedef std::vector<Coordinates> CoordinatesVector;
typedef std::pair<GrainCell *, Coordinates> CellCoordinatesPair;

typedef std::map<RgbColor, bool> ColorMap;

typedef std::map<StateWithCoordinates, unsigned> NeighbourStateCountMap;

struct CoordinatesCellComparator {
    bool operator()(const CellCoordinatesPair &lhs, const CellCoordinatesPair &rhs) const {
        return lhs.first < rhs.first;
    }
};

typedef std::set<CellCoordinatesPair, CoordinatesCellComparator> CoordinatesCellSet;

typedef std::vector<std::thread> ThreadVector;

#endif //WIELOSKALOWE_TYPES_H
