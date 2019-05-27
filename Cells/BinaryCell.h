#ifndef CELL_H
#define CELL_H

#include <QColor>
#include <Types/Types.h>


class BinaryCell {
public:
    BinaryCell() = default;

    explicit BinaryCell(bool isFake);

    void setState(unsigned newState);

    void changeState();

    unsigned getState() const;

    QColor getColor() const;

    bool isFake() const;

    const RealCoordinates &getCenterOfGravity() const;

protected:
    unsigned state{0};
    QColor colors[2] = {Qt::yellow, Qt::red};
    bool fake{false};
};

#endif //CELL_H
