#include <cmath>
#include "GrainGrowthWindow.h"
#include "ui_graingrowthwindow.h"

GrainGrowthWindow::GrainGrowthWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::GrainGrowthWindow) {
    ui->setupUi(this);
    ui->celluralTable->setModel(&gridModel);
    ui->celluralTable->setFocusPolicy(Qt::NoFocus);
    ui->celluralTable->setSelectionMode(QAbstractItemView::NoSelection);
    connect(ui->celluralTable, SIGNAL(clicked(const QModelIndex &)), &gridModel, SLOT(onCellSelected(const QModelIndex &)));
    connect(ui->startingCompositionSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(onStartingCompositionChanged(int)));
    ui->celluralTable->verticalHeader()->setVisible(false);
    ui->celluralTable->horizontalHeader()->setVisible(false);

    for (int i = 0; i < ui->stackedWidget->count(); ++i) {
        QSizePolicy::Policy policy = QSizePolicy::Ignored;
        if (i == ui->stackedWidget->currentIndex()) {
            policy = QSizePolicy::Expanding;
        }
        ui->stackedWidget->widget(i)->setSizePolicy(policy, policy);
    }
}

GrainGrowthWindow::~GrainGrowthWindow() {
    delete ui;
}

void GrainGrowthWindow::on_drawButton_clicked() {
    auto height = static_cast<unsigned short>(ui->heightInput->value());
    auto width = static_cast<unsigned short>(ui->widthInput->value());
    auto startingComposition = ui->startingCompositionSelect->currentText();
    int size = static_cast<int>(floor(1. * ui->celluralTable->width() / width));
    size = std::min(size, 50);
    ui->celluralTable->verticalHeader()->setDefaultSectionSize(size);
    ui->celluralTable->horizontalHeader()->setDefaultSectionSize(size);

    gridModel.drawGrid(height, width);
    if (startingComposition == "Jednorodny") {
        auto rows = ui->grainRowsInput->value();
        auto columns = ui->grainColumnsInput->value();
        gridModel.setHomogeneousComposition(rows, columns);
    } else if (startingComposition == "Losowy") {
        auto count = ui->randomGrainCountInput->value();
        gridModel.setRandomComposition(count);
    } else if (startingComposition == "Losowy z promieniem") {
        auto count = ui->randomWithRadiusGrainCountInput->value();
        auto radius = ui->radiusInput->value();
        gridModel.setRandomComposition(count, radius);
    }
}

void GrainGrowthWindow::on_simulateButton_clicked() {
    auto periodicalBc = ui->periodicalBcRadioButton->isChecked();
    gridModel.startSimulation(periodicalBc ? BoundaryCondition::PERIODICAL : BoundaryCondition::ABSORBING);
}

void GrainGrowthWindow::onStartingCompositionChanged(int currentIndex) {
    ui->stackedWidget->currentWidget()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->stackedWidget->setCurrentIndex(currentIndex);
    ui->stackedWidget->currentWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
