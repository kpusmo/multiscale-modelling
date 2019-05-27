#ifndef WIELOSKALOWE_TWODIMENSIONALNEIGHBOURHOODSERVICE_H
#define WIELOSKALOWE_TWODIMENSIONALNEIGHBOURHOODSERVICE_H

#include <Neighbourhood/NeighbourhoodService.h>

template<typename T>
class TwoDimensionalNeighbourhoodService : public NeighbourhoodService<T> {
public:
    CoordinatesVector getCellNeighbourCoordinates(const Grid<T> &grid, int i, int j) override;
};

template<typename T>
CoordinatesVector TwoDimensionalNeighbourhoodService<T>::getCellNeighbourCoordinates(const Grid<T> &grid, int i, int j) {
    CoordinatesVector neighbourCoordinates;
    auto localNeighbourhood = this->getLocalNeighbourhood();

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
            for (int y = i - this->neighbourhoodRadius; y < i + this->neighbourhoodRadius; ++y) {
                for (int x = j - this->neighbourhoodRadius; x < j + this->neighbourhoodRadius; ++x) {
                    if (y == i && x == j) {
                        continue;
                    }
                    const auto &cell = grid[y][x];
                    auto state = cell.getState();
                    if (this->mode == NeighbourhoodService<T>::ZERO_ONLY && state != 0) {
                        continue;
                    }
                    if (this->mode == NeighbourhoodService<T>::NON_ZERO_ONLY && state == 0) {
                        continue;
                    }
                    if (this->omitFakes && cell.isFake()) {
                        continue;
                    }
                    if (state == this->stateToOmit) {
                        continue;
                    }
                    auto currentCellGravityCenter = cell.getCenterOfGravity();
                    double xx = x + currentCellGravityCenter.first;
                    double yy = y + currentCellGravityCenter.second;
                    auto distance = sqrt(pow(yy - cy, 2) + pow(xx - cx, 2));
                    if (distance < this->neighbourhoodRadius) {
                        neighbourCoordinates.emplace_back(y, x);
                    }
                }
            }
            return neighbourCoordinates;
        }
    }
    if (this->mode == NeighbourhoodService<T>::ALL && this->stateToOmit == -1) {
        return neighbourCoordinates;
    }
    auto coordinates = neighbourCoordinates.begin();
    while (coordinates != neighbourCoordinates.end()) {
        const auto &cell = grid[coordinates->first][coordinates->second];
        if (this->mode == NeighbourhoodService<T>::NON_ZERO_ONLY && cell.getState() == 0) {
            coordinates = neighbourCoordinates.erase(coordinates);
            continue;
        }
        if (this->mode == NeighbourhoodService<T>::ZERO_ONLY && cell.getState() != 0) {
            coordinates = neighbourCoordinates.erase(coordinates);
            continue;
        }
        if (this->stateToOmit == cell.getState()) {
            coordinates = neighbourCoordinates.erase(coordinates);
            continue;
        }
        if (this->omitFakes && cell.isFake()) {
            coordinates = neighbourCoordinates.erase(coordinates);
            continue;
        }
        ++coordinates;
    }
    return neighbourCoordinates;
}

#endif //WIELOSKALOWE_TWODIMENSIONALNEIGHBOURHOODSERVICE_H
