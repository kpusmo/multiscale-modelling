#include <cmath>
#include <QMessageBox>
#include "GrainGrowthWindow.h"
#include "ui_graingrowthwindow.h"

GrainGrowthWindow::GrainGrowthWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::GrainGrowthWindow) {
    ui->setupUi(this);
    initCelluralTable();
    initCompositionInputGroup();
    initNeighbourhoodInputGroup();
    connect(&gridModel, SIGNAL(showMessageBox(const QString &)), this, SLOT(showMessageBox(const QString &)));
}

GrainGrowthWindow::~GrainGrowthWindow() {
    delete ui;
}

void GrainGrowthWindow::on_drawButton_clicked() {
    auto height = static_cast<unsigned short>(ui->heightInput->value());
    auto width = static_cast<unsigned short>(ui->widthInput->value());
    auto startingComposition = ui->startingCompositionSelect->currentText();
    int size = std::min(ui->celluralTable->width() / width, 50);
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
    if (neighbourhood == Neighbourhood::RADIUS) {
        auto neighbourhoodRadius = ui->radiusNeighbourhoodInput->value();
        gridModel.setNeighbourhoodRadius(neighbourhoodRadius);
    }
    gridModel.startSimulation(periodicalBc ? BoundaryCondition::PERIODICAL : BoundaryCondition::ABSORBING);
}

Neighbourhood GrainGrowthWindow::getChosenNeighbourhood() {
    auto neighbourhoodOption = ui->neighbourhoodSelect->currentText();
    if (neighbourhoodOption == "Heksagonalne") {
        auto index = ui->hexagonalSelect->currentIndex();
        auto type = ui->hexagonalSelect->currentText();
        if (type == "Losowo") {
            return Neighbourhood::HEXAGONAL_RANDOM;
        }
        return HEXAGONALS[index];
    } else if (neighbourhoodOption == "Pentagonalne") {
        auto index = ui->pentagonalSelect->currentIndex();
        auto type = ui->pentagonalSelect->currentText();
        if (type == "Losowe") {
            return Neighbourhood::PENTAGONAL_RANDOM;
        }
        return PENTAGONALS[index];
    } else if (neighbourhoodOption == "Moore'a") {
        return Neighbourhood::MOORE;
    } else if (neighbourhoodOption == "Promień") {
        return Neighbourhood::RADIUS;
    }
    return Neighbourhood::VON_NEUMNANN;
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
}

void GrainGrowthWindow::onNeighbourhoodChanged(int currentIndex) {
    ui->neighbourhoodInputGroup->currentWidget()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->neighbourhoodInputGroup->setCurrentIndex(currentIndex);
    ui->neighbourhoodInputGroup->currentWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void GrainGrowthWindow::showMessageBox(const QString &message) {
    QMessageBox messageBox;
    messageBox.setText(message);
    messageBox.exec();
}
