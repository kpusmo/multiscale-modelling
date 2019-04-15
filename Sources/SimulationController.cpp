#include "Box.h"
#include "SimulationController.h"

SimulationController::SimulationController(Ui::MainWindow *u) : ui(u), simulationSteps(0), cellCount(0) {}

void SimulationController::runSimulation(unsigned short rule) {
    if (simulationSteps == 0) {
        return;
    }
    Box::isCheckingAvailable = false;
    clearGrid(cellCount);
    ui->gridLayoutWidget->resize(ui->gridLayoutWidget->width(), 50 * simulationSteps);
    for (int stepCounter = 0; stepCounter < simulationSteps - 1; ++stepCounter) {
        for (unsigned short cellCounter = 0; cellCounter < cellCount; ++cellCounter) {
            unsigned short cellsMask = grid[stepCounter][cellCounter + 1].getState();
            cellsMask += (grid[stepCounter][cellCounter].getState() << 1);
            cellsMask += (grid[stepCounter][cellCounter - 1].getState() << 2);
            unsigned short ruleMask = rule >> cellsMask;
            grid[stepCounter + 1][cellCounter].setState(static_cast<unsigned short>(ruleMask & 1));
            addBox(static_cast<unsigned short>(stepCounter + 1), cellCounter);
        }
    }
}

void SimulationController::drawFirstRow(unsigned short rows, unsigned short columns) {
    clearGrid();
    Box::isCheckingAvailable = true;
    simulationSteps = static_cast<unsigned short>(rows + 1);
    cellCount = columns;
    grid.reset(simulationSteps, cellCount);
    ui->gridLayoutWidget->resize(ui->gridLayoutWidget->width(), 50);
    for (int i = 0; i < cellCount; ++i) {
        addBox(0, i);
    }
}

void SimulationController::addBox(unsigned short row, unsigned short column) {
    Cell *cell = &grid[row][column];
    QWidget *box = new Box(cell);
    QPalette palette;
    palette.setColor(QPalette::Background, cell->getColor());
    box->setAutoFillBackground(true);
    box->setPalette(palette);
    box->setAttribute(Qt::WA_DeleteOnClose, true);
    ui->gridContainer->addWidget(box, row, column);
}

void SimulationController::clearGrid(unsigned short fromIndex) {
    QLayoutItem *item;
    while ((item = ui->gridContainer->takeAt(fromIndex)) != nullptr) {
        delete item->widget();
        delete item;
    }
}
