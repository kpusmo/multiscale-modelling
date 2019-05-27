#include "OneDimensionalProcessor.h"
#include <Neighbourhood/OneDimensionalNeighbourhoodService.h>

OneDimensionalProcessor::OneDimensionalProcessor() : Processor(new OneDimensionalNeighbourhoodService<BinaryCell>) {}

bool OneDimensionalProcessor::process(Grid<BinaryCell> &grid) {
    if (grid.getWidth() == 0 || grid.getHeight() == 0) {
        return false;
    }
    for (int stepCounter = 0; stepCounter < grid.getHeight() - 1; ++stepCounter) {
        for (auto i = 0; i < grid.getWidth(); ++i) {
            auto neighboursCoordinates = getNeighbourhoodService()->getCellNeighbourCoordinates(grid, stepCounter, i);
            unsigned cellMask = 0;
            for (unsigned k = 0; k < neighboursCoordinates.size(); ++k) {
                const auto &coordinates = neighboursCoordinates[k];
                cellMask += grid[coordinates.first][coordinates.second].getState() << k;
            }
            unsigned ruleMask = rule >> cellMask;
            grid[stepCounter + 1][i].setState(ruleMask & 1);
        }
    }
    return true;
}

NeighbourhoodService<BinaryCell> *OneDimensionalProcessor::getNeighbourhoodService() {
    return neighbourhoodService->reset()->setNeighbourhood(Neighbourhood::VON_NEUMNANN);
}

void OneDimensionalProcessor::reset() {}

OneDimensionalProcessor *OneDimensionalProcessor::setRule(unsigned r) {
    rule = r;
    return this;
}
