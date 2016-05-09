#include "fcartoonizer.h"
#include "ui_fcartoonizer.h"

fCartoonizer::fCartoonizer(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::fCartoonizer)
{
    ui->setupUi(this);
}

fCartoonizer::~fCartoonizer()
{
    delete ui;
}
