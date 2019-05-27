#ifndef WIELOSKALOWE_GAMEGRIDMODEL_H
#define WIELOSKALOWE_GAMEGRIDMODEL_H


#include <Cells/BinaryCell.h>
#include <AbstractGridModel/GridModel.h>
#include <QTimer>

class GameGridModel : public GridModel<BinaryCell> {
Q_OBJECT
public:
    static const unsigned TIMER_INTERVAL{200}; //milliseconds

    GameGridModel();

    ~GameGridModel() override;

    void drawGrid(unsigned height, unsigned width, const QString &startingComposition);

    void startSimulation();

public slots:
    void nextStep();

protected:
    bool isRunning{false};
    QTimer *timer{nullptr};

    bool isCellSelectionAvailable() override;

    void setStartingComposition(const QString &startingComposition);

    void stopSimulation();
};


#endif //WIELOSKALOWE_GAMEGRIDMODEL_H