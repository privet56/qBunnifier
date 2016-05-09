#ifndef W_H
#define W_H

#include <QWidget>

class w : public QWidget
{
    Q_OBJECT
public:
    explicit w(QWidget *parent = 0);
    static QString getOpenFileName(QWidget* parent, QString sTitle, QString sPattern);

signals:

public slots:
};

#endif // W_H
