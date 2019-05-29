#ifndef WIELOSKALOWE_ONEDIMENSIONALNEIGHBOURHOODSERVICE_H
#define WIELOSKALOWE_ONEDIMENSIONALNEIGHBOURHOODSERVICE_H

#include <Neighbourhood/Service/NeighbourhoodService.h>

template<typename T>
class OneDimensionalNeighbourhoodService : public NeighbourhoodService<T> {
public:
    CoordinatesVector getCellNeighbourCoordinates(const NeighbourhoodTransferObject<T> *dto) override;
    
    ~OneDimensionalNeighbourhoodService() override = default;
};

template<typename T>
CoordinatesVector OneDimensionalNeighbourhoodService<T>::getCellNeighbourCoordinates(const NeighbourhoodTransferObject<T> *dto) {
    auto coordinates = dto->getCoordinates();
    CoordinatesVector neighbourCoordinates;
    switch (dto->getNeighbourhood()) {
        case Neighbourhood::VON_NEUMNANN:
            neighbourCoordinates.emplace_back(coordinates.first, coordinates.second - 1);
            neighbourCoordinates.emplace_back(coordinates.first, coordinates.second);
            neighbourCoordinates.emplace_back(coordinates.first, coordinates.second + 1);
            break;
            //TODO rest of neighbourhoods, modes, etc.
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
