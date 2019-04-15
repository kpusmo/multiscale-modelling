#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GridModel.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:

    void on_startButton_clicked();

    void on_drawButton_clicked();

private:
    Ui::MainWindow *ui;
    GridModel gridModel;
};

#endif // MAINWINDOW_H
