#ifndef OCV_H
#define OCV_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>

#include <QObject>
#include <QImage>

using namespace std;
using namespace cv;

class ocv : public QObject
{
    Q_OBJECT
public:
    explicit ocv(QObject *parent = 0);

    static QImage MatToQImage(const Mat& mat);

signals:

public slots:
};

#endif // OCV_H
