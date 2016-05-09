#ifndef FCARTOONIZER_H
#define FCARTOONIZER_H

#include <QFrame>

namespace Ui {
class fCartoonizer;
}

class fCartoonizer : public QFrame
{
    Q_OBJECT

public:
    explicit fCartoonizer(QWidget *parent = 0);
    ~fCartoonizer();

private:
    Ui::fCartoonizer *ui;
};

#endif // FCARTOONIZER_H
