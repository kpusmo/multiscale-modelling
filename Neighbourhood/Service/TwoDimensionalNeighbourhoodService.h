#ifndef WIELOSKALOWE_TWODIMENSIONALNEIGHBOURHOODSERVICE_H
#define WIELOSKALOWE_TWODIMENSIONALNEIGHBOURHOODSERVICE_H

#include <Neighbourhood/Service/NeighbourhoodService.h>

template<typename T>
class TwoDimensionalNeighbourhoodService : public NeighbourhoodService<T> {
public:
    CoordinatesVector getCellNeighbourCoordinates(const NeighbourhoodTransferObject<T> *dto) override;

    ~TwoDimensionalNeighbourhoodService() override = default;
};

template<typename T>
CoordinatesVector TwoDimensionalNeighbourhoodService<T>::getCellNeighbourCoordinates(const NeighbourhoodTransferObject<T> *dto) {
    auto c = dto->getCoordinates();
    int i = c.first, j = c.second;
    CoordinatesVector neighbourCoordinates;
    auto mode = dto->getMode();
    auto omitFakes = dto->isIgnoringFakes();
    auto stateToOmit = dto->getStateToOmit();
    auto neighbourhoodRadius = dto->getRadius();
    const Grid<T> &grid = dto->getGrid();

    switch (dto->getNeighbourhood()) {
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
            double cy = i + centerOfGravity.first;
            double cx = j + centerOfGravity.second;
            for (int y = i - neighbourhoodRadius; y <= i + neighbourhoodRadius; ++y) {
                for (int x = j - neighbourhoodRadius; x <= j + neighbourhoodRadius; ++x) {
                    if (y == i && x == j) {
                        continue;
                    }
                    const auto &cell = grid[y][x];
                    auto state = cell.getState();
                    if (mode == NeighbourhoodTransferObjectMode::ZERO_ONLY && state != 0) {
                        continue;
                    }
                    if (mode == NeighbourhoodTransferObjectMode::NON_ZERO_ONLY && state == 0) {
                        continue;
                    }
                    if (omitFakes && cell.isFake()) {
                        continue;
                    }
                    if (state == stateToOmit) {
                        continue;
                    }
                    auto cellCellGravityCenter = cell.getCenterOfGravity();
                    double yy = y + cellCellGravityCenter.first;
                    double xx = x + cellCellGravityCenter.second;
                    auto distance = sqrt(pow(yy - cy, 2) + pow(xx - cx, 2));
                    if (distance <= neighbourhoodRadius) {
                        neighbourCoordinates.emplace_back(y, x);
                    }
                }
            }
            return neighbourCoordinates;
        }
    }
    if (mode == NeighbourhoodTransferObjectMode::ALL && stateToOmit == -1) {
        return neighbourCoordinates;
    }
    auto coordinates = neighbourCoordinates.begin();
    while (coordinates != neighbourCoordinates.end()) {
        const auto &cell = grid[coordinates->first][coordinates->second];
        if (mode == NeighbourhoodTransferObjectMode::NON_ZERO_ONLY && cell.getState() == 0) {
            coordinates = neighbourCoordinates.erase(coordinates);
            continue;
        }
        if (mode == NeighbourhoodTransferObjectMode::ZERO_ONLY && cell.getState() != 0) {
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

#endif //WIELOSKALOWE_TWODIMENSIONALNEIGHBOURHOODSERVICE_H
