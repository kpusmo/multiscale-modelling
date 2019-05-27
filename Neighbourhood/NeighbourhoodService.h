#ifndef WIELOSKALOWE_NEIGHBOURHOODSERVICE_H
#define WIELOSKALOWE_NEIGHBOURHOODSERVICE_H


#include "Neighbourhood.h"
#include <Grid/Grid.h>
#include <Types/Types.h>

template<typename T>
class NeighbourhoodService {
public:
    enum Mode {
        ALL,
        NON_ZERO_ONLY,
        ZERO_ONLY
    };

    NeighbourhoodService() : randomNumberGenerator(randomDevice()) {}

    NeighbourhoodService *setNeighbourhood(const Neighbourhood &newNeighbourhood) {
        neighbourhood = newNeighbourhood;
        return this;
    }

    NeighbourhoodService *setRadius(int radius) {
        neighbourhoodRadius = radius;
        return this;
    }

    NeighbourhoodService *setMode(Mode newMode) {
        mode = newMode;
        return this;
    }

    NeighbourhoodService *omitState(int state) {
        stateToOmit = state;
        return this;
    }

    NeighbourhoodService *ignoreFakes(bool ignore) {
        omitFakes = ignore;
        return this;
    }

    NeighbourhoodService *reset() {
        stateToOmit = -1;
        mode = Mode::ALL;
        neighbourhoodRadius = 0;
        omitFakes = false;
        return this;
    }

    virtual CoordinatesVector getCellNeighbourCoordinates(const Grid<T> &grid, int i, int j) = 0;

protected:
    Neighbourhood neighbourhood{};
    int neighbourhoodRadius{};
    int stateToOmit{-1};
    Mode mode{ALL};
    bool omitFakes{false};
    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator;

    Neighbourhood getLocalNeighbourhood() {
        if (neighbourhood != Neighbourhood::PENTAGONAL_RANDOM && neighbourhood != Neighbourhood::HEXAGONAL_RANDOM) {
            return neighbourhood;
        }
        if (neighbourhood == Neighbourhood::PENTAGONAL_RANDOM) {
            std::uniform_int_distribution<> distribution(0, 3);
            return PENTAGONALS[distribution(randomNumberGenerator)];
        }
        //Neighbourhood::HEXAGONAL_RANDOM
        std::uniform_int_distribution<> distribution(0, 1);
        return HEXAGONALS[distribution(randomNumberGenerator)];
    }
};


#endif //WIELOSKALOWE_NEIGHBOURHOODSERVICE_H
