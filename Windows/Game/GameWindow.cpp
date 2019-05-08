#include "GameWindow.h"
#include "ui_gamewindow.h"
#include <cmath>

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::GameWindow) {
    ui->setupUi(this);
    ui->celluralTable->setModel(&gridModel);
    ui->celluralTable->setFocusPolicy(Qt::NoFocus);
    ui->celluralTable->setSelectionMode(QAbstractItemView::NoSelection);
    connect(ui->celluralTable, SIGNAL(clicked(const QModelIndex &)), &gridModel, SLOT(onCellSelected(const QModelIndex &)));
    ui->celluralTable->verticalHeader()->setVisible(false);
    ui->celluralTable->horizontalHeader()->setVisible(false);
}

GameWindow::~GameWindow() {
    delete ui;
}

void GameWindow::on_drawButton_clicked() {
    auto height = static_cast<unsigned short>(ui->heightInput->value());
    auto width = static_cast<unsigned short>(ui->widthInput->value());
    int size = static_cast<int>(floor(1. * ui->celluralTable->width() / width));
    size = std::min(size, 50);
    ui->celluralTable->verticalHeader()->setDefaultSectionSize(size);
    ui->celluralTable->horizontalHeader()->setDefaultSectionSize(size);
    auto startingComposition = ui->startingCompositionSelect->currentText();
    gridModel.drawGrid(height, width, startingComposition);
}

void GameWindow::on_simulateButton_clicked() {
    gridModel.startSimulation();
}
