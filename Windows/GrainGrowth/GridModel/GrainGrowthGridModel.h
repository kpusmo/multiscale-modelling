#ifndef WIELOSKALOWE_GRAINGROWTHGRIDMODEL_H
#define WIELOSKALOWE_GRAINGROWTHGRIDMODEL_H


#include <AbstractGridModel/GridModel.h>
#include <Cells/GrainCell.h>
#include <QTimer>
#include <Grid/Neighbourhood.h>
#include <memory>

class GrainGrowthGridModel : public GridModel<GrainCell> {
Q_OBJECT
public:
    static const unsigned TIMER_INTERVAL{200}; //milliseconds

    GrainGrowthGridModel();

    ~GrainGrowthGridModel() override;

    void startSimulation(const BoundaryCondition &bc);

    void simulate() override;

    void drawGrid(unsigned short height, unsigned short width);

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

public slots:

    void nextStep();

    void onCellSelected(const QModelIndex &index) override;

protected:
    Grid<GrainCell> previousState;
    bool isRunning{false};
    QTimer *timer{nullptr};
    Neighbourhood neighbourhood{Neighbourhood::VON_NEUMNANN};

    typedef std::map<GrainCell, unsigned short> GrainCellMap;
    typedef std::shared_ptr<GrainCellMap> GrainCellMapPointer;

    bool isCellSelectionAvailable() override;

    void stopSimulation();

    const GrainCell *findMostFrequentNeighbourCell(int i, int j) const;

    void markCellWithNeighboursAsUnavailable(int a, int b, bool **map, int radius);

    std::vector<std::pair<int, int>> getCoordinatesOfAvailableCells(bool **map);

    bool **getGridMapForRandomComposition(int radius);

    GrainCellMapPointer getNeighbourStateMap(int i, int j) const;
};


#endif //WIELOSKALOWE_GRAINGROWTHGRIDMODEL_H
