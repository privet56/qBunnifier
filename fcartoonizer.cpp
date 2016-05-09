#include "fcartoonizer.h"
#include "ui_fcartoonizer.h"

#include <QTimer>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QDesktopWidget>

#include "ocv.h"
#include "str.h"
#include "w.h"
#include "frabbifiermuzzlemodel.h"
#include "frabbifiermuzzleitemdelegate.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"

#include "overlaynose.h"

fCartoonizer::fCartoonizer(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::fCartoonizer)
{
    ui->setupUi(this);

    this->ui->lineEdit->setText(qApp->applicationDirPath()+"/demo_files/faces5.png");
    QTimer::singleShot(999, this, SLOT(on_lineEdit_returnPressed()));
}

fCartoonizer::~fCartoonizer()
{
    delete ui;
}

void fCartoonizer::on_pushButton_clicked()
{
    QString sAbsFN = w::getOpenFileName(this, tr("Open Image"), tr("Image Files (*.png *.jpg *.bmp)"));

    if(str::isempty(sAbsFN, true))return;

    this->ui->lineEdit->setText(sAbsFN);

    QImage qimg(sAbsFN);
    this->setImage(qimg);
}

void fCartoonizer::on_commandLinkButton_clicked()
{
    if(!QFile::exists(this->ui->lineEdit->text()))
        return;

    QString sFacesPic(this->ui->lineEdit->text());
    string facesPic = sFacesPic.toStdString();
    cv::Mat resultImg = cartoonize(facesPic);
    QImage qimg = ocv::MatToQImage(resultImg);
    this->setImage(qimg);
}

void fCartoonizer::on_lineEdit_returnPressed()
{
    on_commandLinkButton_clicked();
}

void fCartoonizer::on_lineEdit_textChanged(const QString &arg1)
{
    this->ui->commandLinkButton->setEnabled(QFile::exists(arg1));
}

void fCartoonizer::setImage(QImage& qimg)
{
    QRect r = QApplication::desktop()->screenGeometry();
    int maxHeight = r.height() - 333;
    //qDebug() << "desktopHeight:" << r.height() << " maxHeight:" << maxHeight << " imgHeight:" << qimg.height();
    while((qimg.width() > r.width()) || (qimg.height() > maxHeight))
    {
        qimg = qimg.scaled(r.width(), maxHeight, Qt::KeepAspectRatio);
    }
    QPixmap pxm = QPixmap::fromImage(qimg);
    this->ui->label->setPixmap(pxm);
}

void fCartoonizer::setEnv(logger* pLog)
{
    this->m_pLog = pLog;
}
