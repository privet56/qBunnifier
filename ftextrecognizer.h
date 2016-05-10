#ifndef FTEXTRECOGNIZER_H
#define FTEXTRECOGNIZER_H

#include <QFrame>
#include "logger.h"

namespace Ui {
class fTextRecognizer;
}

class fTextRecognizer : public QFrame
{
    Q_OBJECT

public:
    explicit fTextRecognizer(QWidget *parent = 0);
    ~fTextRecognizer();
    void setEnv(logger* pLog);

protected:
    logger* m_pLog;
    void setImage(QImage& img);

private slots:
    void on_lineEdit_returnPressed();
    void on_lineEdit_textChanged(const QString &arg1);
    void on_pushButton_clicked();
    void on_commandLinkButton_clicked();

private:
    Ui::fTextRecognizer *ui;
};

#endif // FTEXTRECOGNIZER_H
