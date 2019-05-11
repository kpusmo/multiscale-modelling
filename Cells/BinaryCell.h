#ifndef CELL_H
#define CELL_H

#include <QColor>


class BinaryCell {
public:
    BinaryCell() = default;

    void setState(unsigned short newState);

    void changeState();

    short getState() const;

    QColor getColor() const;

protected:
    unsigned short state{0};
    QColor colors[2] = {Qt::yellow, Qt::red};
};

#endif //CELL_H
