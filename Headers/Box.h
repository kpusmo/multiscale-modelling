#ifndef BOX_H
#define BOX_H

#include "Cell.h"

#include <QWidget>
#include <QEvent>

class Box : public QWidget {
Q_OBJECT
public:
    static bool isCheckingAvailable;

    Box(QWidget *parent = nullptr) = delete;

    explicit Box(Cell *cell, QWidget *parent = nullptr);

    bool event(QEvent *event);

protected:
    Cell *cell;
};

#endif // BOX_H
