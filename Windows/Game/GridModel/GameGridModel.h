#ifndef WIELOSKALOWE_GAMEGRIDMODEL_H
#define WIELOSKALOWE_GAMEGRIDMODEL_H


#include <Cells/BinaryCell.h>
#include <AbstractGridModel/GridModel.h>
#include <QTimer>

class GameGridModel : public GridModel<BinaryCell> {
Q_OBJECT
public:
    static const unsigned TIMER_INTERVAL{500}; //milliseconds

    GameGridModel();

    ~GameGridModel() override;

    void drawGrid(unsigned short height, unsigned short width, const QString &startingComposition);

    void startSimulation();

    void simulate() override;

public slots:
    void nextStep();

protected:
    bool isCellSelectionAvailable() override;

    unsigned countLivingSurroundingCells(int i, int j);

    void setStartingComposition(const QString &startingComposition);

    Grid<BinaryCell> previousState;
    bool isRunning{false};
    QTimer *timer{nullptr};

    void stopSimulation();
};


#endif //WIELOSKALOWE_GAMEGRIDMODEL_H