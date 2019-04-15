#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include "ui_mainwindow.h"
#include "Grid.h"

class SimulationController {
public:
    SimulationController() = delete;

    explicit SimulationController(Ui::MainWindow *ui);

    void drawFirstRow(unsigned short rows, unsigned short columns);

    void runSimulation(unsigned short rule);

protected:
    void addBox(unsigned short row, unsigned short column);

    void clearGrid(unsigned short fromIndex = 0);

    Ui::MainWindow *ui;
    unsigned short simulationSteps;
    unsigned short cellCount;
    Grid grid;
};


#endif // SIMULATIONCONTROLLER_H
