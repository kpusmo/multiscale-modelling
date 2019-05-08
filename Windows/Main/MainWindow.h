#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Windows/OneDimensional/OneDimensionalWindow.h"
#include "Windows/Game/GameWindow.h"

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

private:
    Ui::MainWindow *ui;
    OneDimensionalWindow *oneDimensionalModeWindow;
    GameWindow *gameModeWindow;
};

#endif // MAINWINDOW_H
