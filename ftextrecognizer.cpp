#include "ftextrecognizer.h"
#include "ui_ftextrecognizer.h"

fTextRecognizer::fTextRecognizer(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::fTextRecognizer)
{
    ui->setupUi(this);
}

fTextRecognizer::~fTextRecognizer()
{
    delete ui;
}
