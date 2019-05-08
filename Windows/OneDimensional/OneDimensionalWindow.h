#ifndef ONEDIMENSIONALWINDOW_H
#define ONEDIMENSIONALWINDOW_H

#include <QMainWindow>
#include "Windows/OneDimensional/GridModel/OneDimensionalGridModel.h"

namespace Ui {
    class OneDimensionalWindow;
}

class OneDimensionalWindow : public QMainWindow {
Q_OBJECT

public:
    explicit OneDimensionalWindow(QWidget *parent = nullptr);

    ~OneDimensionalWindow() override;

private slots:
    void on_startButton_clicked();

    void on_drawButton_clicked();

private:
    Ui::OneDimensionalWindow *ui;
    OneDimensionalGridModel gridModel;
};

#endif // ONEDIMENSIONALWINDOW_H
