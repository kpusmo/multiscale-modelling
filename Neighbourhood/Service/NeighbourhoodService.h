#ifndef WIELOSKALOWE_NEIGHBOURHOODSERVICE_H
#define WIELOSKALOWE_NEIGHBOURHOODSERVICE_H


#include "Neighbourhood/Neighbourhood.h"
#include <Grid/Grid.h>
#include <Types/Types.h>
#include <Neighbourhood/DataTransferObject/NeighbourhoodTransferObject.h>

template<typename T>
class NeighbourhoodService {
public:
    NeighbourhoodService() = default;

    virtual ~NeighbourhoodService() = default;

    virtual CoordinatesVector getCellNeighbourCoordinates(const NeighbourhoodTransferObject<T> *dto) = 0;
};


#endif //WIELOSKALOWE_NEIGHBOURHOODSERVICE_H
