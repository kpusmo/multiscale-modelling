#ifndef GRAINGROWTHWINDOW_H
#define GRAINGROWTHWINDOW_H

#include <QMainWindow>
#include <Windows/GrainGrowth/GridModel/RadiusNeighbourhood/RadiusNeighbourhoodGridModel.h>

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

    void onStartingCompositionChanged(int currentIndex);

    void onNeighbourhoodChanged(int currentIndex);

    void onPostProcessingChanged(int currentIndex);

    void showMessageBox(const QString &message);

private:
    Ui::GrainGrowthWindow *ui;
    RadiusNeighbourhoodGridModel gridModel;

    void initCompositionInputGroup();

    void initNeighbourhoodInputGroup();

    void initCelluralTable();

    Neighbourhood getChosenNeighbourhood();

    void initPostProcessingInputGroup();
};

#endif // GRAINGROWTHWINDOW_H
