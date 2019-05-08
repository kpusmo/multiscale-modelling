#ifndef WIELOSKALOWE_BASEGRIDMODEL_H
#define WIELOSKALOWE_BASEGRIDMODEL_H


#include <QAbstractTableModel>

class BaseGridModel : public QAbstractTableModel {
Q_OBJECT
public:
    explicit BaseGridModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {}

public slots:
    virtual void onCellSelected(const QModelIndex &index) = 0;
};

#endif //WIELOSKALOWE_BASEGRIDMODEL_H
