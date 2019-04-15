#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), sc(ui) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_startButton_clicked() {
    auto rule = static_cast<unsigned short>(ui->ruleInput->value());
    sc.runSimulation(rule);
}

void MainWindow::on_drawButton_clicked() {
    auto cellCount = static_cast<unsigned short>(ui->cellCountInput->value());
    auto simulationSteps = static_cast<unsigned short>(ui->simulationStepsInput->value());
    sc.drawFirstRow(simulationSteps, cellCount);
}
