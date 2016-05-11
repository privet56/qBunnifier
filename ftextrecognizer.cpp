#include "ftextrecognizer.h"
#include "ui_ftextrecognizer.h"
#include "segmentocr.h"
#include "ocr.h"

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

using namespace std;
using namespace cv;

fTextRecognizer::fTextRecognizer(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::fTextRecognizer)
{
    ui->setupUi(this);

    QString sImg(qApp->applicationDirPath()+"/demo_files/text1.png");
    this->ui->lineEdit->setText(sImg);

    {
        QStringList langs;
        QString sTessdataDir = str::makeAbsFN(qApp->applicationDirPath(), "/tessdata");
        QString sPattern = "*.wordlist";

        QDirIterator it(sTessdataDir, QStringList() << sPattern, QDir::Files);
        while (it.hasNext())
        {
            QString sLang(it.next());
            QFileInfo finfo(sLang);
            langs << finfo.baseName();
        }
        this->ui->cbLang->addItems(langs);
        this->ui->cbLang->setCurrentText("eng");
    }
}

fTextRecognizer::~fTextRecognizer()
{
    delete ui;
}

void fTextRecognizer::on_lineEdit_returnPressed()
{
    on_commandLinkButton_clicked();
}

void fTextRecognizer::on_lineEdit_textChanged(const QString &arg1)
{
    bool b = QFile::exists(arg1);
    this->ui->commandLinkButton->setEnabled(b);
    if(b)
    {
        QImage qimg(arg1);
        this->setImage(qimg);
    }
}

void fTextRecognizer::on_pushButton_clicked()
{
    QString sAbsFN = w::getOpenFileName(this, tr("Open Image"), tr("Image Files (*.png *.jpg *.bmp)"));

    if(str::isempty(sAbsFN, true))return;

    this->ui->lineEdit->setText(sAbsFN);
}

void fTextRecognizer::on_commandLinkButton_clicked()
{
    if(!QFile::exists(this->ui->lineEdit->text()))
    {
        return;
    }
//TODO: progress bar & wait dlg
    QString sPic(this->ui->lineEdit->text());
    std::string pic = sPic.toStdString();

    string lang = this->ui->cbLang->currentText().toStdString();

    int iFoundRegions=0;
#ifdef USE_OPENCV_TEXT_MODULE
    std::string txt = main_ocr2(pic, lang, iFoundRegions);
#else
    std::string txt = main_ocr(pic, lang, iFoundRegions);
#endif

    QString sTxt = QString::fromStdString(txt).trimmed();
    this->ui->plainTextEdit->clear();
    this->ui->plainTextEdit->appendPlainText(sTxt);
    this->m_pLog->inf("found text regions:"+QString::number(iFoundRegions)+" text len:"+QString::number(sTxt.length())+" used language:"+QString::fromStdString(lang));
}

void fTextRecognizer::setImage(QImage& qimg)
{
    QRect r = QApplication::desktop()->screenGeometry();
    int maxHeight = (r.height() - 333) / 2;
    int maxWidth  = (r.width()  - 3) / 2;

    while((qimg.width() > maxWidth) || (qimg.height() > maxHeight))
    {
        qimg = qimg.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio);
    }
    QPixmap pxm = QPixmap::fromImage(qimg);
    this->ui->label->setPixmap(pxm);
}

void fTextRecognizer::setEnv(logger* pLog)
{
    this->m_pLog = pLog;
}
