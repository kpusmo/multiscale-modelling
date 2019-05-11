#include "OneDimensionalWindow.h"
#include "ui_onedimensionalwindow.h"
#include <cmath>

OneDimensionalWindow::OneDimensionalWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::OneDimensionalWindow) {
    ui->setupUi(this);
    ui->celluralTable->setModel(&gridModel);
    ui->celluralTable->setFocusPolicy(Qt::NoFocus);
    ui->celluralTable->setSelectionMode(QAbstractItemView::NoSelection);
    connect(ui->celluralTable, SIGNAL(clicked(const QModelIndex &)), &gridModel, SLOT(onCellSelected(const QModelIndex &)));
    ui->celluralTable->verticalHeader()->setVisible(false);
    ui->celluralTable->horizontalHeader()->setVisible(false);
}

OneDimensionalWindow::~OneDimensionalWindow() {
    delete ui;
}

void OneDimensionalWindow::on_startButton_clicked() {
    auto rule = static_cast<unsigned short>(ui->ruleInput->value());
    auto simulationSteps = static_cast<unsigned short>(ui->simulationStepsInput->value());
    gridModel.setSimulationSteps(simulationSteps);
    gridModel.setRule(rule);
    gridModel.simulate();
}

void OneDimensionalWindow::on_drawButton_clicked() {
    auto cellCount = static_cast<unsigned short>(ui->cellCountInput->value());
    auto highStateCount = ui->highStateCountInput->value();
    int size = static_cast<int>(floor(1. * ui->celluralTable->width() / cellCount));
    size = std::min(size, 50);
    ui->celluralTable->verticalHeader()->setDefaultSectionSize(size);
    ui->celluralTable->horizontalHeader()->setDefaultSectionSize(size);
    gridModel.setCellCount(cellCount, highStateCount);
}