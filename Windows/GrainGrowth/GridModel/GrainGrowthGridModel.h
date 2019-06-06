#ifndef WIELOSKALOWE_GRAINGROWTHGRIDMODEL_H
#define WIELOSKALOWE_GRAINGROWTHGRIDMODEL_H


#include <AbstractGridModel/GridModel.h>
#include <Cells/GrainCell.h>
#include <QTimer>
#include <Neighbourhood/Neighbourhood.h>
#include <Neighbourhood/Service/TwoDimensionalNeighbourhoodService.h>
#include <memory>
#include <PostProcessors/MonteCarlo/MonteCarloProcessor.h>
#include <PostProcessors/DRX/DrxProcessor.h>

enum PostProcessing {
    NONE,
    MONTE_CARLO
};

enum SimulationStage {
    SIMULATION_STAGE,
    MONTE_CARLO_STAGE,
    DRX_STAGE
};

enum ViewMode {
    GRAINS,
    ENERGY,
    DISLOCATION
};

class GrainGrowthGridModel : public GridModel<GrainCell> {
Q_OBJECT
public:
    static const unsigned TIMER_INTERVAL{200}; //milliseconds

    GrainGrowthGridModel();

    explicit GrainGrowthGridModel(Processor<GrainCell> *p);

    ~GrainGrowthGridModel() override;

    virtual void startSimulation(const BoundaryCondition &bc);

    void drawGrid(unsigned height, unsigned width);

    void setNeighbourhoodTransferObject(Neighbourhood neighbourhood, int radius);


    /**
     * Sets state of count cells as non-zero. If positive radius provided, there will not be more than one non-zero cell in given radius.
     *
     * @param count
     * @param radius
     */
    void setRandomComposition(int count, int radius = 0);

    /**
     * Sets evenly distributed cells as non-zero. Shrinks parameters to grid's size.
     *
     * @param rows
     * @param columns
     */
    void setHomogeneousComposition(int rows, int columns);

    void setPostProcessing(PostProcessing processing);

    void setMonteCarloKTFactor(double factor);

    void setMonteCarloStepCount(int count);

    void setDrxStepCount(int count);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void toggleViewMode();

    void setDrxTransferObject(DrxTransferObject &dto);

public slots:
    void nextStep();

    void onCellSelected(const QModelIndex &index) override;

signals:
    void showMessageBox(const QString &message);

protected:
    bool isRunning{false};
    QTimer *timer{nullptr};
    MonteCarloProcessor *monteCarloProcessor;
    DrxProcessor *drxProcessor;
    PostProcessing postProcessing{PostProcessing::MONTE_CARLO};
    SimulationStage stage{SIMULATION_STAGE};
    ViewMode viewMode{ViewMode::GRAINS};
    int mcStepCount{};
    int mcSimulationStep{1};
    int drxSimulationStep{};
    int drxStepCount{};

    bool isCellSelectionAvailable() override;

    void stopSimulation();

private:
    void stageEnded();

    void markCellWithNeighboursAsUnavailable(int a, int b, bool **map, int radius);

    CoordinatesVector getCoordinatesOfAvailableCells(bool **map);

    bool **getGridMapForRandomComposition(int radius);

    void simulationStep();

    void monteCarloStep();

    void drxStep();
};


#endif //WIELOSKALOWE_GRAINGROWTHGRIDMODEL_H
