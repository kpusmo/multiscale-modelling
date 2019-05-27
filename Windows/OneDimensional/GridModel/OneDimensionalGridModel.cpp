#include "OneDimensionalGridModel.h"
#include <QDebug>
#include <iostream>
#include <Windows/OneDimensional/Processor/OneDimensionalProcessor.h>

OneDimensionalGridModel::OneDimensionalGridModel() : GridModel(new OneDimensionalProcessor) {}

void OneDimensionalGridModel::setCellCount(unsigned cellCount, int highStateCount) {
    beginResetModel();
    grid.reset(1, cellCount);
    grid.changeRandomCellStates(highStateCount);
    endResetModel();
}

void OneDimensionalGridModel::setSimulationSteps(unsigned simulationSteps) {
    if (grid.getHeight() != 1) { //can set simulation steps only after drawing first row
        return;
    }
    beginResetModel();
    grid.addRows(simulationSteps);
    endResetModel();
}

void OneDimensionalGridModel::simulate() {
    processor->process(grid);
}

bool OneDimensionalGridModel::isCellSelectionAvailable() {
    return grid.getHeight() == 1;
}

void OneDimensionalGridModel::setRule(unsigned r) {
    //TODO that's ugly, do something
    dynamic_cast<OneDimensionalProcessor *>(processor)->setRule(r);
}
