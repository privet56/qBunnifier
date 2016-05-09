#ifndef FRABBIFIERMUZZLEITEMDELEGATE_H
#define FRABBIFIERMUZZLEITEMDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>
#include <QWidget>
#include <QTextEdit>
#include <QStatusBar>
#include "logger.h"

class fRabbifierMuzzleItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit fRabbifierMuzzleItemDelegate(QObject *parent = 0);

    // Create Editor when we construct ourselves
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    // Then, we set the Editor
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    // When we modify data, this model reflect the change
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    // Give our editor the info on size and location
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

signals:

public slots:

};

#endif // FRABBIFIERMUZZLEITEMDELEGATE_H
