#include <MainWindow.h>
#include "ui_mainwindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->celluralTable->setModel(&gridModel);
    ui->celluralTable->setFocusPolicy(Qt::NoFocus);
    ui->celluralTable->setSelectionMode(QAbstractItemView::NoSelection);
    connect(ui->celluralTable, SIGNAL(clicked(const QModelIndex &)), &gridModel, SLOT(onCellSelected(const QModelIndex &)));
    ui->celluralTable->verticalHeader()->setVisible(false);
    ui->celluralTable->horizontalHeader()->setVisible(false);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_startButton_clicked() {
    auto rule = static_cast<unsigned short>(ui->ruleInput->value());
    auto simulationSteps = static_cast<unsigned short>(ui->simulationStepsInput->value());
    gridModel.setSimulationSteps(simulationSteps);
    for (int i = 0; i < simulationSteps + 1; ++i) {
        ui->celluralTable->setRowHeight(i, ui->celluralTable->columnWidth(0));
    }
    gridModel.simulate(rule);
}

void MainWindow::on_drawButton_clicked() {
    auto cellCount = static_cast<unsigned short>(ui->cellCountInput->value());
    auto highStateCount = ui->highStateCountInput->value();
    gridModel.setCellCount(cellCount, highStateCount);
    int size = static_cast<int>(floor(1. * ui->celluralTable->width() / cellCount));
    for (int i = 0; i < cellCount; ++i) {
        ui->celluralTable->setColumnWidth(i, size);
    }
    ui->celluralTable->setRowHeight(0, size);
}