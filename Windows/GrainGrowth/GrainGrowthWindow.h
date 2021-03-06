#ifndef GRAINGROWTHWINDOW_H
#define GRAINGROWTHWINDOW_H

#include <QMainWindow>
#include <Windows/GrainGrowth/GridModel/GrainGrowthGridModel.h>

namespace Ui {
    class GrainGrowthWindow;
}

class GrainGrowthWindow : public QMainWindow {
Q_OBJECT

public:
    explicit GrainGrowthWindow(QWidget *parent = nullptr);

    ~GrainGrowthWindow() override;

public slots:
    void on_drawButton_clicked();

    void on_simulateButton_clicked();

    void on_toggleViewModeButton_clicked();

    void onStartingCompositionChanged(int currentIndex);

    void onNeighbourhoodChanged(int currentIndex);

    void showMessageBox(const QString &message);

private:
    Ui::GrainGrowthWindow *ui;
    GrainGrowthGridModel gridModel;

    void initCompositionInputGroup();

    void initNeighbourhoodInputGroup();

    void initCelluralTable();

    Neighbourhood getChosenNeighbourhood();

    void setNeighbourhood();

    void setMonteCarloData();

    void setDrxData();
};

#endif // GRAINGROWTHWINDOW_H
