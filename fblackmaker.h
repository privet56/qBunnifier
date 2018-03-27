#ifndef FBLACKMAKER_H
#define FBLACKMAKER_H

#include <QFrame>
#include "logger.h"
#include "blackmaker/blackmakerthread.h"

namespace Ui {
class fBlackmaker;
}

class fBlackmaker : public QFrame
{
    Q_OBJECT

public:
    explicit fBlackmaker(QWidget *parent = 0);
    ~fBlackmaker();
    void setEnv(logger* pLog);
    void stop();

protected:
    logger* m_pLog;
    void setImage(QImage& img);
    BlackMakerThread* m_pThread;

private slots:
    void setInImage(QImage img);
    void setOutImage(QImage img);

    void on_pushButton_clicked();

    void on_commandLinkButton_clicked();

    void on_lineEdit_returnPressed();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_checkBox_toggled(bool checked);

private:
    Ui::fBlackmaker *ui;
};

#endif // FBLACKMAKER_H
