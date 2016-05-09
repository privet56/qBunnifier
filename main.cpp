#include "mainwindow.h"
#include <QSplashScreen>
#include <QApplication>
#include <QFile>
#include <QSystemTrayIcon>
#include <QTextStream>
#include "globalinclude.h"
#include "cfg.h"
#include "logger.h"
#include "tray.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//splash
    QPixmap pixmap(":/res/bun.png");
    QSplashScreen splash(pixmap, Qt::SplashScreen | Qt::WindowStaysOnTopHint);
#ifndef _DEBUG
    splash.show();
#endif
    splash.showMessage("Loading...", Qt::AlignBottom, Qt::red);

    cfg _cfg(&a);
    logger _log(&_cfg, nullptr, &a);

//css
    {
        QFile data(":/res/css.css");
        QString style;
        if (data.open(QFile::ReadOnly))
        {
            QTextStream styleIn(&data);
            style = styleIn.readAll();
            data.close();
        }
        a.setStyleSheet(style);
    }

//wnd
    MainWindow w(&splash, &_log);
    w.show();

//tray
    tray trayIcon(QIcon(":/res/bun.png"), &w);
    trayIcon.setup();
    //QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &trayIcon, SLOT(showIfHidden()));

    return a.exec();
}
