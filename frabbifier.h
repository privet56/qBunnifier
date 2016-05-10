#ifndef FRABBIFIER_H
#define FRABBIFIER_H

#include <QFrame>
#include "logger.h"

namespace Ui {
class fRabbifier;
}

class fRabbifier : public QFrame
{
    Q_OBJECT

public:
    explicit fRabbifier(QWidget *parent = 0);
    ~fRabbifier();
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
    Ui::fRabbifier *ui;
};

#endif // FRABBIFIER_H
