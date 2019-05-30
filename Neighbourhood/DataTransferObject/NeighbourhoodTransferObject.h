#ifndef WIELOSKALOWE_NEIGHBOURHOODTRANSFEROBJECT_H
#define WIELOSKALOWE_NEIGHBOURHOODTRANSFEROBJECT_H

#include <random>
#include <Neighbourhood/Neighbourhood.h>
#include <Grid/Grid.h>

enum NeighbourhoodTransferObjectMode {
    ALL,
    NON_ZERO_ONLY,
    ZERO_ONLY
};

template<typename T>
class NeighbourhoodTransferObject {
public:
    NeighbourhoodTransferObject() = delete;

    NeighbourhoodTransferObject(const Grid<T> &g, Neighbourhood n, int radius = 0);

    NeighbourhoodTransferObject(const NeighbourhoodTransferObject &other);

    NeighbourhoodTransferObject &operator=(const NeighbourhoodTransferObject &other);

    const Grid<T> &getGrid() const;

    NeighbourhoodTransferObject *setNeighbourhood(const Neighbourhood &newNeighbourhood);

    NeighbourhoodTransferObject *setRadius(int radius);

    int getRadius() const;

    NeighbourhoodTransferObject *setMode(NeighbourhoodTransferObjectMode newMode);

    NeighbourhoodTransferObjectMode getMode() const;

    NeighbourhoodTransferObject *omitState(int state);

    int getStateToOmit() const;

    NeighbourhoodTransferObject *ignoreFakes(bool ignore);

    bool isIgnoringFakes() const;

    NeighbourhoodTransferObject *setCoordinates(Coordinates c);

    Coordinates getCoordinates() const;

    NeighbourhoodTransferObject *reset();

    Neighbourhood getNeighbourhood() const;

protected:
    const Grid<T> &grid;
    Coordinates coordinates;
    Neighbourhood neighbourhood;
    int neighbourhoodRadius;
    NeighbourhoodTransferObjectMode mode{ALL};
    bool omitFakes{false};
    int stateToOmit{-1};
};

template<typename T>
NeighbourhoodTransferObject<T>::NeighbourhoodTransferObject(const Grid<T> &g, Neighbourhood n, int radius) : grid(g), neighbourhood(n), neighbourhoodRadius(radius) {}

template<typename T>
NeighbourhoodTransferObject<T>::NeighbourhoodTransferObject(const NeighbourhoodTransferObject<T> &other) : grid(other.grid) {
    neighbourhood = other.neighbourhood;
    neighbourhoodRadius = other.neighbourhoodRadius;
    coordinates = other.coordinates;
    mode = other.mode;
    omitFakes = other.omitFakes;
    stateToOmit = other.stateToOmit;
}

template<typename T>
NeighbourhoodTransferObject<T> &NeighbourhoodTransferObject<T>::operator=(const NeighbourhoodTransferObject<T> &other) {
    neighbourhood = other.neighbourhood;
    neighbourhoodRadius = other.neighbourhoodRadius;
    coordinates = other.coordinates;
    mode = other.mode;
    omitFakes = other.omitFakes;
    stateToOmit = other.stateToOmit;
}

template<typename T>
const Grid<T> &NeighbourhoodTransferObject<T>::getGrid() const {
    return grid;
}

template<typename T>
NeighbourhoodTransferObject<T> *NeighbourhoodTransferObject<T>::setNeighbourhood(const Neighbourhood &newNeighbourhood) {
    neighbourhood = newNeighbourhood;
    return this;
}

template<typename T>
NeighbourhoodTransferObject<T> *NeighbourhoodTransferObject<T>::setRadius(int radius) {
    neighbourhoodRadius = radius;
    return this;
}

template<typename T>
int NeighbourhoodTransferObject<T>::getRadius() const {
    return neighbourhoodRadius;
}

template<typename T>
NeighbourhoodTransferObject<T> *NeighbourhoodTransferObject<T>::setMode(NeighbourhoodTransferObjectMode newMode) {
    mode = newMode;
    return this;
}

template<typename T>
NeighbourhoodTransferObjectMode NeighbourhoodTransferObject<T>::getMode() const {
    return mode;
}

template<typename T>
NeighbourhoodTransferObject<T> *NeighbourhoodTransferObject<T>::omitState(int state) {
    stateToOmit = state;
    return this;
}

template<typename T>
int NeighbourhoodTransferObject<T>::getStateToOmit() const {
    return stateToOmit;
}

template<typename T>
NeighbourhoodTransferObject<T> *NeighbourhoodTransferObject<T>::ignoreFakes(bool ignore) {
    omitFakes = ignore;
    return this;
}

template<typename T>
bool NeighbourhoodTransferObject<T>::isIgnoringFakes() const {
    return omitFakes;
}

template<typename T>
NeighbourhoodTransferObject<T> *NeighbourhoodTransferObject<T>::setCoordinates(Coordinates c) {
    coordinates = c;
    return this;
}

template<typename T>
Coordinates NeighbourhoodTransferObject<T>::getCoordinates() const {
    return coordinates;
}

template<typename T>
NeighbourhoodTransferObject<T> *NeighbourhoodTransferObject<T>::reset() {
    stateToOmit = -1;
    mode = NeighbourhoodTransferObjectMode::ALL;
    omitFakes = false;
    return this;
}

template<typename T>
Neighbourhood NeighbourhoodTransferObject<T>::getNeighbourhood() const {
    if (neighbourhood != Neighbourhood::PENTAGONAL_RANDOM && neighbourhood != Neighbourhood::HEXAGONAL_RANDOM) {
        return neighbourhood;
    }
    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator(randomDevice());
    if (neighbourhood == Neighbourhood::PENTAGONAL_RANDOM) {
        std::uniform_int_distribution<> distribution(0, 3);
        return PENTAGONALS[distribution(randomNumberGenerator)];
    }
    //Neighbourhood::HEXAGONAL_RANDOM
    std::uniform_int_distribution<> distribution(0, 1);
    return HEXAGONALS[distribution(randomNumberGenerator)];
}



#endif //WIELOSKALOWE_NEIGHBOURHOODTRANSFEROBJECT_H
