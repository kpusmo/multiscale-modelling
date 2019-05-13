#ifndef WIELOSKALOWE_GAMEWINDOW_H
#define WIELOSKALOWE_GAMEWINDOW_H


#include <QMainWindow>
#include <Windows/Game/GridModel/GameGridModel.h>

namespace Ui {
    class GameWindow;
}

class GameWindow : public QMainWindow {
Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);

    ~GameWindow() override;

private slots:
    void on_simulateButton_clicked();

    void on_drawButton_clicked();

private:
    Ui::GameWindow *ui;
    GameGridModel gridModel;
};

#endif //WIELOSKALOWE_GAMEWINDOW_H
