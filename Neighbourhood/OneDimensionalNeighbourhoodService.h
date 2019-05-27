#ifndef WIELOSKALOWE_ONEDIMENSIONALNEIGHBOURHOODSERVICE_H
#define WIELOSKALOWE_ONEDIMENSIONALNEIGHBOURHOODSERVICE_H

#include <Neighbourhood/NeighbourhoodService.h>

template<typename T>
class OneDimensionalNeighbourhoodService : public NeighbourhoodService<T> {
public:
    CoordinatesVector getCellNeighbourCoordinates(const Grid<T> &grid, int i, int j) override;
};

template<typename T>
CoordinatesVector OneDimensionalNeighbourhoodService<T>::getCellNeighbourCoordinates(const Grid<T> &grid, int i, int j) {
    CoordinatesVector neighbourCoordinates;
    auto localNeighbourhood = this->getLocalNeighbourhood();
    switch (localNeighbourhood) {
        case Neighbourhood::VON_NEUMNANN:
            neighbourCoordinates.emplace_back(i, j - 1);
            neighbourCoordinates.emplace_back(i, j);
            neighbourCoordinates.emplace_back(i, j + 1);
            break;
        //TODO rest of neighbourhoods
        case MOORE:break;
        case RADIUS:break;
        case PENTAGONAL_TOP:break;
        case PENTAGONAL_RIGHT:break;
        case PENTAGONAL_BOTTOM:break;
        case PENTAGONAL_LEFT:break;
        case PENTAGONAL_RANDOM:break;
        case HEXAGONAL_LEFT_TOP:break;
        case HEXAGONAL_RIGHT_TOP:break;
        case HEXAGONAL_RANDOM:break;
    }
    return neighbourCoordinates;
}

#endif //WIELOSKALOWE_ONEDIMENSIONALNEIGHBOURHOODSERVICE_H
