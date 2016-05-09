#include "w.h"
#include "str.h"

#include <QApplication>
#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>

w::w(QWidget *parent) : QWidget(parent)
{

}

QString w::getOpenFileName(QWidget* parent, QString sTitle, QString sPattern)
{
    static QString sDir(qApp->applicationDirPath());
    QString sAbsFN = QFileDialog::getOpenFileName(parent, sTitle, sDir, sPattern);

    if(!str::isempty(sAbsFN, true))
    {
        QFileInfo finfo(sAbsFN);
        sDir = finfo.path();
    }

    return sAbsFN;
}
