#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Windows/OneDimensional/OneDimensionalWindow.h"
#include "Windows/Game/GameWindow.h"
#include "Windows/GrainGrowth/GrainGrowthWindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:
    void on_oneDimensionalModeButton_clicked();
    void on_gameModeButton_clicked();
    void on_grainGrowthButton_clicked();

private:
    Ui::MainWindow *ui;
    OneDimensionalWindow *oneDimensionalModeWindow;
    GameWindow *gameModeWindow;
    GrainGrowthWindow *grainGrowthWindow;
};

#endif // MAINWINDOW_H
