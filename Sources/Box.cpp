#include "Box.h"

bool Box::isCheckingAvailable = true;

Box::Box(Cell *c, QWidget *parent) : cell(c), QWidget(parent) {}

bool Box::event(QEvent *event) {
    if (event->type() == QEvent::MouseButtonRelease && Box::isCheckingAvailable) {
        cell->setState(static_cast<unsigned short>((cell->getState() + 1) % 2));
        QPalette palette;
        palette.setColor(QPalette::Background, cell->getColor());
        setPalette(palette);
        return true;
    }
    return QWidget::event(event);
}
