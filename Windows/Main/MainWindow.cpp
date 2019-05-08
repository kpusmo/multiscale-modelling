#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), oneDimensionalModeWindow(nullptr), gameModeWindow(nullptr) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
    delete oneDimensionalModeWindow;
    delete gameModeWindow;
}

void MainWindow::on_oneDimensionalModeButton_clicked() {
    if (oneDimensionalModeWindow != nullptr) {
        oneDimensionalModeWindow->show();
        return;
    }
    oneDimensionalModeWindow = new OneDimensionalWindow;
    oneDimensionalModeWindow->show();
}


void MainWindow::on_gameModeButton_clicked() {
    if (gameModeWindow != nullptr) {
        gameModeWindow->show();
        return;
    }
    gameModeWindow = new GameWindow;
    gameModeWindow->show();
}
