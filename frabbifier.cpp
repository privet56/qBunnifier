#include "frabbifier.h"
#include "ui_frabbifier.h"

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

fRabbifier::fRabbifier(QWidget *parent) :
    QFrame(parent),
    m_pLog(nullptr),
    ui(new Ui::fRabbifier)
{
    ui->setupUi(this);

    this->ui->lineEdit->setText(qApp->applicationDirPath()+"/demo_files/faces5.png");
    QTimer::singleShot(999, this, SLOT(on_lineEdit_returnPressed()));

    {
        this->ui->tableView;
        fRabbifierMuzzleModel* pfRabbifierMuzzleModel = new fRabbifierMuzzleModel(this->ui->tableView);
        this->ui->tableView->setModel(pfRabbifierMuzzleModel);
        this->ui->tableView->selectRow(0);
        this->ui->tableView->resizeColumnsToContents();
        this->ui->tableView->resizeRowsToContents();
        //this->ui->tableView->setItemDelegate(new fRabbifierMuzzleItemDelegate(this->ui->tableView));
    }
}

fRabbifier::~fRabbifier()
{
    delete ui;
}

void fRabbifier::on_pushButton_clicked()
{
    QString sAbsFN = w::getOpenFileName(this, tr("Open Image"), tr("Image Files (*.png *.jpg *.bmp)"));

    if(str::isempty(sAbsFN, true))return;

    this->ui->lineEdit->setText(sAbsFN);

    QImage qimg(sAbsFN);
    this->setImage(qimg);
}

void fRabbifier::on_commandLinkButton_clicked()
{
    if(!QFile::exists(this->ui->lineEdit->text()))
        return;

    {
        QString s1(qApp->applicationDirPath()+"/res/haarcascade_frontalface_alt.xml");
        QString s2(qApp->applicationDirPath()+"/res/haarcascade_mcs_nose.xml");
        QString sFacesPic(this->ui->lineEdit->text());
        QModelIndexList sels = this->ui->tableView->selectionModel()->selectedRows();
        QString idx = QString::number(sels.at(0).row());
        QString s3(qApp->applicationDirPath()+"/res/muzzle"+idx+".png");
        string faceCascadeName = s1.toStdString();
        string noseCascadeName = s2.toStdString();
        string sAbsFNFacesPic = sFacesPic.toStdString();
        string sAbsFNPic = s3.toStdString();

        int iRecognizedFaces = 0;
        int iRecognizedNoses = 0;

        cv::Mat resultImg = main_overlayNose(faceCascadeName, noseCascadeName, sAbsFNFacesPic, sAbsFNPic, iRecognizedFaces, iRecognizedNoses);
        QImage qimg = ocv::MatToQImage(resultImg);
        this->setImage(qimg);
        this->m_pLog->log("found faces: "+QString::number(iRecognizedFaces)+" with noses:"+QString::number(iRecognizedNoses), iRecognizedNoses > 0 ? logger::LogLevel::INF : logger::LogLevel::WRN);
    }
}

void fRabbifier::setEnv(logger* pLog)
{
    this->m_pLog = pLog;
    ((fRabbifierMuzzleModel*)this->ui->tableView->model())->setEnv(pLog);
}

void fRabbifier::on_lineEdit_returnPressed()
{
    on_commandLinkButton_clicked();
}

void fRabbifier::on_lineEdit_textChanged(const QString &arg1)
{
    this->ui->commandLinkButton->setEnabled(QFile::exists(arg1));
}

void fRabbifier::setImage(QImage& qimg)
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
