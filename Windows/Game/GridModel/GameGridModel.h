#ifndef WIELOSKALOWE_GAMEGRIDMODEL_H
#define WIELOSKALOWE_GAMEGRIDMODEL_H


#include <Grid/Cell.h>
#include <AbstractGridModel/GridModel.h>
#include <QTimer>

class GameGridModel : public GridModel<Cell> {
Q_OBJECT
public:
    static const unsigned TIMER_INTERVAL{500}; //milliseconds

    GameGridModel();

    ~GameGridModel() override;

    void drawGrid(unsigned short height, unsigned short width, const QString &startingComposition);

    void startSimulation();

    void simulate() override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public slots:
    void incrementResultIndex();

protected:
    bool isCellSelectionAvailable() override;

    unsigned countLivingSurroundingCells(int i, int j);

    std::vector<Grid<Cell>> results;
    int currentResult{-1};
    QTimer *timer{nullptr};

    void startTimer();

    void setStartingComposition(const QString &startingComposition);
};


#endif //WIELOSKALOWE_GAMEGRIDMODEL_H