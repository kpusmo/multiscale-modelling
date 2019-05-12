#include <cmath>
#include "GrainGrowthWindow.h"
#include "ui_graingrowthwindow.h"

GrainGrowthWindow::GrainGrowthWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::GrainGrowthWindow) {
    ui->setupUi(this);
    initCelluralTable();
    initCompositionInputGroup();
    initNeighbourhoodInputGroup();
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
    auto neighbourhood = getChosenNeighbourhood();
    gridModel.setNeighbourhood(neighbourhood);
    gridModel.startSimulation(periodicalBc ? BoundaryCondition::PERIODICAL : BoundaryCondition::ABSORBING);
}

Neighbourhood GrainGrowthWindow::getChosenNeighbourhood() {
    auto neighbourhoodOption = ui->neighbourhoodSelect->currentText();
    Neighbourhood neighbourhood = Neighbourhood::VON_NEUMNANN;
    std::random_device rd;
    std::mt19937 generator(rd());
    if (neighbourhoodOption == "Heksagonalne") {
        auto index = ui->hexagonalSelect->currentIndex();
        auto type = ui->hexagonalSelect->currentText();
        if (type == "Losowo") {
            std::uniform_int_distribution<> distribution(0, 1);
            index = distribution(generator);
        }
        neighbourhood = HEXAGONALS[index];
    } else if (neighbourhoodOption == "Pentagonalne") {
        auto index = ui->pentagonalSelect->currentIndex();
        auto type = ui->pentagonalSelect->currentText();
        if (type == "Losowe") {
            std::uniform_int_distribution<> distribution(0, 3);
            index = distribution(generator);
        }
        neighbourhood = PENTAGONALS[index];
    } else if (neighbourhoodOption == "Moore'a") {
        neighbourhood = Neighbourhood::MOORE;
    }
    return neighbourhood;
}

void GrainGrowthWindow::onStartingCompositionChanged(int currentIndex) {
    ui->compositionInputGroup->currentWidget()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->compositionInputGroup->setCurrentIndex(currentIndex);
    ui->compositionInputGroup->currentWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void GrainGrowthWindow::initCompositionInputGroup() {
    connect(ui->startingCompositionSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(onStartingCompositionChanged(int)));
    for (int i = 0; i < ui->compositionInputGroup->count(); ++i) {
        QSizePolicy::Policy policy = QSizePolicy::Ignored;
        if (i == ui->compositionInputGroup->currentIndex()) {
            policy = QSizePolicy::Expanding;
        }
        ui->compositionInputGroup->widget(i)->setSizePolicy(policy, policy);
    }
}

void GrainGrowthWindow::initNeighbourhoodInputGroup() {
    connect(ui->neighbourhoodSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(onNeighbourhoodChanged(int)));
    for (int i = 0; i < ui->neighbourhoodInputGroup->count(); ++i) {
        QSizePolicy::Policy policy = QSizePolicy::Ignored;
        if (i == ui->neighbourhoodInputGroup->currentIndex()) {
            policy = QSizePolicy::Expanding;
        }
        ui->neighbourhoodInputGroup->widget(i)->setSizePolicy(policy, policy);
    }
}

void GrainGrowthWindow::initCelluralTable() {
    ui->celluralTable->setModel(&gridModel);
    ui->celluralTable->setFocusPolicy(Qt::NoFocus);
    ui->celluralTable->setSelectionMode(QAbstractItemView::NoSelection);
    connect(ui->celluralTable, SIGNAL(clicked(const QModelIndex &)), &gridModel, SLOT(onCellSelected(const QModelIndex &)));
    ui->celluralTable->verticalHeader()->setVisible(false);
    ui->celluralTable->horizontalHeader()->setVisible(false);
}

void GrainGrowthWindow::onNeighbourhoodChanged(int currentIndex) {
    ui->neighbourhoodInputGroup->currentWidget()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->neighbourhoodInputGroup->setCurrentIndex(currentIndex);
    ui->neighbourhoodInputGroup->currentWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
