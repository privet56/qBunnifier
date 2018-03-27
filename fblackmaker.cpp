#include "fBlackmaker.h"
#include "ui_fblackmaker.h"

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

using namespace std;
using namespace cv;

fBlackmaker::fBlackmaker(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::fBlackmaker),
    m_pThread(nullptr)
{
    ui->setupUi(this);

    this->ui->lineEdit->setText(qApp->applicationDirPath()+"/demo_files/trump1.mp4");

    //QTimer::singleShot(999, this, SLOT(on_lineEdit_returnPressed()));
}

fBlackmaker::~fBlackmaker()
{
    delete ui;
}

void fBlackmaker::on_pushButton_clicked()
{
    QString sAbsFN = w::getOpenFileName(this, tr("Open Video"), tr("Image Files (*.avi *.mp4)"));

    if(str::isempty(sAbsFN, true))return;

    this->ui->lineEdit->setText(sAbsFN);
}

void fBlackmaker::on_commandLinkButton_clicked()
{
    if(!QFile::exists(this->ui->lineEdit->text()))
        return;

    this->stop();
    {
        m_pThread = new BlackMakerThread(this->ui->lineEdit->text(), this->m_pLog, this);
        connect(m_pThread, SIGNAL(finished()), m_pThread, SLOT(start()));
        connect(m_pThread, SIGNAL(inDisplay(QImage)), this, SLOT(setInImage(QImage)));
        connect(m_pThread, SIGNAL(outDisplay(QImage)), this, SLOT(setOutImage(QImage)));
        m_pThread->start();
    }
}

void fBlackmaker::stop()
{
    if (m_pThread)
    {
        m_pThread->disconnect();
        m_pThread->requestInterruption();
        m_pThread->terminate();
        QThread::msleep(300);
        m_pThread->quit();
        QThread::msleep(300);
        //m_pThread->quit();
        //m_pThread->deleteLater();
        //m_pThread->wait();
        m_pThread = nullptr;
    }
}

void fBlackmaker::on_lineEdit_returnPressed()
{
    on_commandLinkButton_clicked();
}

void fBlackmaker::on_lineEdit_textChanged(const QString &arg1)
{
    this->ui->commandLinkButton->setEnabled(QFile::exists(arg1));
}

void fBlackmaker::setInImage(QImage qimg)
{
    if(this->ui->checkBox->isChecked())
        return;
    this->setImage(qimg);
}
void fBlackmaker::setOutImage(QImage qimg)
{
    if(!this->ui->checkBox->isChecked())
        return;
    this->setImage(qimg);
}

void fBlackmaker::setImage(QImage& qimg)
{
#ifdef USESCREENSIZE
    QRect r = QApplication::desktop()->screenGeometry();
    int maxHeight = r.height() - 333;
#else
    QRect r = this->geometry();
    int maxHeight = r.height() -133;
#endif

    //qDebug() << "desktopHeight:" << r.height() << " maxHeight:" << maxHeight << " imgHeight:" << qimg.height();
    while((qimg.width() > r.width()) || (qimg.height() > maxHeight))
    {
        qimg = qimg.scaled(r.width(), maxHeight, Qt::KeepAspectRatio);
    }
    QPixmap pxm = QPixmap::fromImage(qimg);
    this->ui->label->setPixmap(pxm);
}

void fBlackmaker::setEnv(logger* pLog)
{
    this->m_pLog = pLog;
}

void fBlackmaker::on_checkBox_toggled(bool checked)
{

}
