#ifndef BLACKMAKERTHREAD_H
#define BLACKMAKERTHREAD_H

#include <QObject>
#include <QThread>
#include <QPixmap>
#include "opencv2/opencv.hpp"
#include "logger.h"

class BlackMakerThread : public QThread
{
    Q_OBJECT
public:
    explicit BlackMakerThread(QString sVidFN, logger* pLog, QObject *parent = nullptr);
public:
    QString m_sVidFN;
    void setEnv(logger* pLog);
protected:
    logger* m_pLog;
    void runVid();
public:
    static void make(cv::Mat* pInFrame, cv::Mat* pOutFrame);
    static void make();

    static void rgbHist(const cv::Mat &inputImage, cv::Mat &outputImage);
    static void hsvHist(const cv::Mat &inputImage, cv::Mat &outputImage);

signals:
    void inDisplay(QImage image);
    void outDisplay(QImage image);

public slots:

private:
    void run() override;

};

#endif // BLACKMAKERTHREAD_H
