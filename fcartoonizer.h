#ifndef FCARTOONIZER_H
#define FCARTOONIZER_H

#include <QFrame>
#include "logger.h"

namespace Ui {
class fCartoonizer;
}

class fCartoonizer : public QFrame
{
    Q_OBJECT

public:
    explicit fCartoonizer(QWidget *parent = 0);
    ~fCartoonizer();
    void setEnv(logger* pLog);

protected:
    logger* m_pLog;
    void setImage(QImage& img);

private slots:
    void on_pushButton_clicked();

    void on_commandLinkButton_clicked();

    void on_lineEdit_returnPressed();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::fCartoonizer *ui;
};

#endif // FCARTOONIZER_H
