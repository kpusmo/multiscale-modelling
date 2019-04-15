#ifndef CELL_H
#define CELL_H

#include <QColor>


class Cell {
public:
    Cell();

    void setState(unsigned short newState);

    unsigned short getState() const {
        return state;
    }

    QColor getColor() const;

protected:
    unsigned short state;
    QColor colors[2] = {Qt::yellow, Qt::red};
};

#endif //CELL_H
