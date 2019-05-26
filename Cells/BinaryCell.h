#ifndef CELL_H
#define CELL_H

#include <QColor>


class BinaryCell {
public:
    BinaryCell() = default;

    explicit BinaryCell(bool isFake);

    void setState(unsigned newState);

    void changeState();

    int getState() const;

    QColor getColor() const;

protected:
    unsigned state{0};
    QColor colors[2] = {Qt::yellow, Qt::red};
    bool fake{false};
};

#endif //CELL_H
