#ifndef FRABBIFIERMUZZLEMODEL_H
#define FRABBIFIERMUZZLEMODEL_H

#include <QAbstractTableModel>
#include "logger.h"

class fRabbifierMuzzleModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit fRabbifierMuzzleModel(QObject *parent = 0);
    ~fRabbifierMuzzleModel();

    void setEnv(logger* pLog);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QModelIndex buddy(const QModelIndex &index) const;
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

protected:
    logger* m_pLog;

signals:

public slots:

};

#endif // FRABBIFIERMUZZLEMODEL_H
