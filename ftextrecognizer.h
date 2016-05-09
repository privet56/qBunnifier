#ifndef FTEXTRECOGNIZER_H
#define FTEXTRECOGNIZER_H

#include <QFrame>

namespace Ui {
class fTextRecognizer;
}

class fTextRecognizer : public QFrame
{
    Q_OBJECT

public:
    explicit fTextRecognizer(QWidget *parent = 0);
    ~fTextRecognizer();

private:
    Ui::fTextRecognizer *ui;
};

#endif // FTEXTRECOGNIZER_H
