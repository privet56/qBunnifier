#include "mainwindow.h"
#include "tray.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>

#include "ocv.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"

#include "overlaynose.h"

MainWindow::MainWindow(QSplashScreen* pSplash, logger* pLog, QWidget *parent) :
    QMainWindow(parent),
    m_pLog(pLog),
    m_pSplash(pSplash),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->ui->Rabbifier->setEnv(this->m_pLog);
    this->ui->eLog->SetStatusBar(this->statusBar());
    this->m_pLog->SetLogWnd(this->ui->eLog);

    QTimer::singleShot(999, pSplash, SLOT(close()));
    this->m_pLog->inf("App Startup Finished");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionMinimize_triggered()
{
    tray::getTrayIcon()->hide_show();
}

void MainWindow::on_actionAbout_triggered()
{
    tray::getTrayIcon()->onAbout();
}

void MainWindow::on_actionShow_Mascot_triggered()
{
    this->m_pSplash->showMessage(" ");
    this->m_pSplash->show();
}
