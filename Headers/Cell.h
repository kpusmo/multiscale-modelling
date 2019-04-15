#ifndef CELL_H
#define CELL_H

#include <QColor>


class Cell {
public:
    Cell() = default;

    void setState(unsigned short newState);

    short getState() const;

    QColor getColor() const;

protected:
    unsigned short state{0};
    QColor colors[2] = {Qt::yellow, Qt::red};
};

#endif //CELL_H
