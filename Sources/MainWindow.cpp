
#include <MainWindow.h>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->celluralTable->setModel(&gridModel);
    ui->celluralTable->setFocusPolicy(Qt::NoFocus);
    ui->celluralTable->setSelectionMode(QAbstractItemView::NoSelection);
    connect(ui->celluralTable, SIGNAL(clicked(const QModelIndex &)), &gridModel, SLOT(onCellSelected(const QModelIndex &)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_startButton_clicked() {
    auto rule = static_cast<unsigned short>(ui->ruleInput->value());
    auto simulationSteps = static_cast<unsigned short>(ui->simulationStepsInput->value());
    gridModel.setSimulationSteps(simulationSteps);
    gridModel.simulate(rule);
}

void MainWindow::on_drawButton_clicked() {
    auto cellCount = static_cast<unsigned short>(ui->cellCountInput->value());
    gridModel.setCellCount(cellCount);
}
