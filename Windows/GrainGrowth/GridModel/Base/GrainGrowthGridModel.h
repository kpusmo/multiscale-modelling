#ifndef WIELOSKALOWE_GRAINGROWTHGRIDMODEL_H
#define WIELOSKALOWE_GRAINGROWTHGRIDMODEL_H


#include <AbstractGridModel/GridModel.h>
#include <Cells/GrainCell.h>
#include <QTimer>
#include <Neighbourhood/Neighbourhood.h>
#include <Neighbourhood/NeighbourhoodService.h>
#include <memory>
#include <Windows/GrainGrowth/MonteCarloProcessor/MonteCarloProcessor.h>

enum PostProcessing {
    NONE,
    MONTE_CARLO
};

enum SimulationStage {
    SIMULATION_STAGE,
    MONTE_CARLO_STAGE
};

class GrainGrowthGridModel : public GridModel<GrainCell> {
Q_OBJECT
public:
    static const unsigned TIMER_INTERVAL{200}; //milliseconds

    GrainGrowthGridModel();

    ~GrainGrowthGridModel() override;

    virtual void startSimulation(const BoundaryCondition &bc);

    void simulate() override;

    void drawGrid(unsigned height, unsigned width);

    void setNeighbourhood(const Neighbourhood &newNeighbourhood);

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

public slots:
    void nextStep();

    void onCellSelected(const QModelIndex &index) override;

signals:
    void showMessageBox(const QString &message);

protected:
    Grid<GrainCell> previousState;
    bool isRunning{false};
    QTimer *timer{nullptr};
    Neighbourhood neighbourhood{Neighbourhood::VON_NEUMNANN};
    NeighbourhoodService *neighbourhoodService;
    MonteCarloProcessor *monteCarloProcessor;
    PostProcessing postProcessing{PostProcessing::MONTE_CARLO};
    SimulationStage stage{SIMULATION_STAGE};
    double monteCarloKTFactor;
    int mcStepCount;
    int mcSimulationStep{1};

    bool isCellSelectionAvailable() override;

    void stopSimulation();

    void monteCarloStep();

    void simulationEnded();

    virtual NeighbourhoodService *getNeighbourhoodService();

private:
    const GrainCell *findMostFrequentNeighbourCell(int i, int j);

    void markCellWithNeighboursAsUnavailable(int a, int b, bool **map, int radius);

    std::vector<std::pair<int, int>> getCoordinatesOfAvailableCells(bool **map);

    bool **getGridMapForRandomComposition(int radius);

    void initCellNeighbourMap(int i, int j);
};


#endif //WIELOSKALOWE_GRAINGROWTHGRIDMODEL_H
