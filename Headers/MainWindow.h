#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SimulationController.h"
#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow() override;

private slots:

    void on_startButton_clicked();

    void on_drawButton_clicked();

private:
    Ui::MainWindow *ui;
    SimulationController sc;
};

#endif // MAINWINDOW_H
