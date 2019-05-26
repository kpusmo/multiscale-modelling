#ifndef WIELOSKALOWE_NEIGHBOURHOODSERVICE_H
#define WIELOSKALOWE_NEIGHBOURHOODSERVICE_H


#include "Neighbourhood.h"
#include <Grid/Grid.h>
#include <Types/Types.h>

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

    template<typename T>
    CoordinatesVector getCellNeighbourCoordinates(const Grid<T> &grid, int i, int j);

protected:
    Neighbourhood neighbourhood{};
    int neighbourhoodRadius{};
    int stateToOmit{-1};
    Mode mode{ALL};
    bool omitFakes{false};
    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator;

    Neighbourhood getLocalNeighbourhood(Neighbourhood neighbourhood) {
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

template<typename T>
CoordinatesVector NeighbourhoodService::getCellNeighbourCoordinates(const Grid<T> &grid, int i, int j) {
    CoordinatesVector neighbourCoordinates;
    auto localNeighbourhood = getLocalNeighbourhood(neighbourhood);

    switch (localNeighbourhood) {
        case Neighbourhood::VON_NEUMNANN:
            neighbourCoordinates.emplace_back(i, j - 1);
            neighbourCoordinates.emplace_back(i, j + 1);
            neighbourCoordinates.emplace_back(i - 1, j);
            neighbourCoordinates.emplace_back(i + 1, j);
            break;
        case Neighbourhood::MOORE:
            for (int a = i - 1; a < i + 2; ++a) {
                for (int b = j - 1; b < j + 2; ++b) {
                    if (!(i == a && j == b)) {
                        neighbourCoordinates.emplace_back(a, b);
                    }
                }
            }
            break;
        case Neighbourhood::HEXAGONAL_LEFT_TOP:
            neighbourCoordinates.emplace_back(i - 1, j - 1);
            neighbourCoordinates.emplace_back(i - 1, j);
            neighbourCoordinates.emplace_back(i, j - 1);

            neighbourCoordinates.emplace_back(i, j + 1);
            neighbourCoordinates.emplace_back(i + 1, j + 1);
            neighbourCoordinates.emplace_back(i + 1, j);
            break;
        case Neighbourhood::HEXAGONAL_RIGHT_TOP:
            neighbourCoordinates.emplace_back(i - 1, j);
            neighbourCoordinates.emplace_back(i - 1, j + 1);
            neighbourCoordinates.emplace_back(i, j + 1);

            neighbourCoordinates.emplace_back(i, j - 1);
            neighbourCoordinates.emplace_back(i + 1, j - 1);
            neighbourCoordinates.emplace_back(i + 1, j);
            break;
        case Neighbourhood::PENTAGONAL_TOP:
            neighbourCoordinates.emplace_back(i, j - 1);
            neighbourCoordinates.emplace_back(i, j + 1);
            neighbourCoordinates.emplace_back(i - 1, j - 1);
            neighbourCoordinates.emplace_back(i - 1, j);
            neighbourCoordinates.emplace_back(i - 1, j + 1);
            break;
        case Neighbourhood::PENTAGONAL_RIGHT:
            neighbourCoordinates.emplace_back(i - 1, j);
            neighbourCoordinates.emplace_back(i + 1, j);
            neighbourCoordinates.emplace_back(i - 1, j + 1);
            neighbourCoordinates.emplace_back(i, j + 1);
            neighbourCoordinates.emplace_back(i + 1, j + 1);
            break;
        case Neighbourhood::PENTAGONAL_BOTTOM:
            neighbourCoordinates.emplace_back(i, j - 1);
            neighbourCoordinates.emplace_back(i, j + 1);
            neighbourCoordinates.emplace_back(i + 1, j - 1);
            neighbourCoordinates.emplace_back(i + 1, j);
            neighbourCoordinates.emplace_back(i + 1, j + 1);
            break;
        case Neighbourhood::PENTAGONAL_LEFT:
            neighbourCoordinates.emplace_back(i - 1, j);
            neighbourCoordinates.emplace_back(i + 1, j);
            neighbourCoordinates.emplace_back(i - 1, j - 1);
            neighbourCoordinates.emplace_back(i, j - 1);
            neighbourCoordinates.emplace_back(i + 1, j - 1);
            break;
        case Neighbourhood::RADIUS: {
            auto centerOfGravity = grid[i][j].getCenterOfGravity();
            double cx = j + centerOfGravity.first;
            double cy = i + centerOfGravity.second;
            for (int y = i - neighbourhoodRadius; y < i + neighbourhoodRadius; ++y) {
                for (int x = j - neighbourhoodRadius; x < j + neighbourhoodRadius; ++x) {
                    if (y == i && x == j) {
                        continue;
                    }
                    const auto &cell = grid[y][x];
                    auto state = cell.getState();
                    if (mode == Mode::ZERO_ONLY && state != 0) {
                        continue;
                    }
                    if (mode == Mode::NON_ZERO_ONLY && state == 0) {
                        continue;
                    }
                    if (omitFakes && cell.isFake()) {
                        continue;
                    }
                    if (state == stateToOmit) {
                        continue;
                    }
                    auto currentCellGravityCenter = cell.getCenterOfGravity();
                    double xx = x + currentCellGravityCenter.first;
                    double yy = y + currentCellGravityCenter.second;
                    auto distance = sqrt(pow(yy - cy, 2) + pow(xx - cx, 2));
                    if (distance < neighbourhoodRadius) {
                        neighbourCoordinates.emplace_back(y, x);
                    }
                }
            }
            return neighbourCoordinates;
        }
    }
    if (mode == Mode::ALL && stateToOmit == -1) {
        return neighbourCoordinates;
    }
    auto coordinates = neighbourCoordinates.begin();
    while (coordinates != neighbourCoordinates.end()) {
        const auto &cell = grid[coordinates->first][coordinates->second];
        if (mode == Mode::NON_ZERO_ONLY && cell.getState() == 0) {
            coordinates = neighbourCoordinates.erase(coordinates);
            continue;
        }
        if (mode == Mode::ZERO_ONLY && cell.getState() != 0) {
            coordinates = neighbourCoordinates.erase(coordinates);
            continue;
        }
        if (stateToOmit == cell.getState()) {
            coordinates = neighbourCoordinates.erase(coordinates);
            continue;
        }
        if (omitFakes && cell.isFake()) {
                coordinates = neighbourCoordinates.erase(coordinates);
                continue;
        }
        ++coordinates;
    }
    return neighbourCoordinates;
}


#endif //WIELOSKALOWE_NEIGHBOURHOODSERVICE_H
