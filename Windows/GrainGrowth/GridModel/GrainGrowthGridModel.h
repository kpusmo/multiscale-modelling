#ifndef WIELOSKALOWE_GRAINGROWTHGRIDMODEL_H
#define WIELOSKALOWE_GRAINGROWTHGRIDMODEL_H


#include <AbstractGridModel/GridModel.h>
#include <Cells/GrainCell.h>
#include <QTimer>

class GrainGrowthGridModel : public GridModel<GrainCell> {
Q_OBJECT
public:
    static const unsigned TIMER_INTERVAL{200}; //milliseconds

    GrainGrowthGridModel();

    ~GrainGrowthGridModel() override;

    void startSimulation(const BoundaryCondition &bc);

    void simulate() override;

    void drawGrid(unsigned short height, unsigned short width);

    void setRandomComposition(int count, int radius = 0);

    void setHomogeneousComposition(int rows, int columns);

public slots:

    void nextStep();

    void onCellSelected(const QModelIndex &index) override;

protected:
    bool isCellSelectionAvailable() override;

    void stopSimulation();

    const GrainCell *findMostFrequentNeighbourCell(int i, int j) const;

    void markNeighboursAsUnavailable(int a, int b, bool **map, int radius);

    std::vector<GrainCell *> cellsAvailableByRadius(int radius);

    Grid<GrainCell> previousState;
    bool isRunning{false};
    QTimer *timer{nullptr};
};


#endif //WIELOSKALOWE_GRAINGROWTHGRIDMODEL_H
