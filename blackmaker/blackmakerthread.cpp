#include "blackmakerthread.h"

BlackMakerThread::BlackMakerThread(QString sVidFN, logger* pLog, QObject *parent) : QThread(parent), m_sVidFN(sVidFN), m_pLog(pLog)
{

}

void BlackMakerThread::setEnv(logger* pLog)
{
    this->m_pLog = pLog;
}

void BlackMakerThread::run()
{
    while(!isInterruptionRequested())
    {
        runVid();
        this->sleep(333);
    }
}
void BlackMakerThread::runVid()
{
    using namespace cv;
    //VideoCapture camera(0);
    VideoCapture camera = VideoCapture(m_sVidFN.toStdString());
    if(!camera.isOpened())
    {
        this->m_pLog->err("Cannot open '"+m_sVidFN+"'");
    }
    Mat inFrame, outFrame;
    while(camera.isOpened() && !isInterruptionRequested())
    {
        camera >> inFrame;
        if(inFrame.empty())
            continue;

        BlackMakerThread::make(&inFrame, &outFrame);

        emit inDisplay(
                        QImage(
                            inFrame.data,
                            inFrame.cols,
                            inFrame.rows,
                            inFrame.step,
                            QImage::Format_RGB888)
                        .rgbSwapped());

        emit outDisplay(
                        QImage(
                            outFrame.data,
                            outFrame.cols,
                            outFrame.rows,
                            outFrame.step,
                            QImage::Format_RGB888)
                        .rgbSwapped());
    }
}

void BlackMakerThread::make(cv::Mat* pInFrame, cv::Mat* pOutFrame)
{
    using namespace cv;

    bitwise_not(*pInFrame, *pOutFrame);

    //TODO:

/*
    int bins = 256;
        int channels[] = {0}; // the first and the only channel
        int histSize[] = { bins }; // number of bins

        float rangeGray[] = {0,255}; // range of grayscale
        const float* ranges[] = { rangeGray };

        Mat histogram;

        calcHist(&grayImg,
           1, // number of images
           channels,
           Mat(), // no masks, an empty Mat
           histogram,
           1, // dimensionality
           histSize,
           ranges,
           true, // uniform
           false // not accumulate
        );
    /*
    Mat image(25, 180, CV_8UC3);
    for(int i=0; i<image.rows; i++)
    {
      for(int j=0; j<image.cols; j++)
      {
        image.at<Vec3b>(i,j)[0] = j;
        image.at<Vec3b>(i,j)[1] = 255;
        image.at<Vec3b>(i,j)[2] = 255;
      }
    }
    cvtColor(image,image,CV_HSV2BGR);
    imshow("Hue", image);
    * /
    Mat hsvImg;
    cvtColor(*pInFrame, hsvImg, CV_BGR2HSV);

    int bins = 24;
    Mat colors(1, bins, CV_8UC3);
    for(int i=0; i<bins; i++)
    {
        colors.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>((i+1)*180.0/bins), 255, 255);
    }
    cvtColor(colors, colors, COLOR_HSV2BGR);

    Point p1(0,0), p2(0,pInFrame->rows-1);

    int bins = 256;

    Mat histogram;
    int histSize[] = { bins }; // number of bins
    float rangeGray[] = {0,255}; // range of grayscale
    const float* ranges[] = { rangeGray };

    calcHist(pInFrame,
       1, // number of images
       channels,
       Mat(), // no masks, an empty Mat
       histogram,
       1, // dimensionality
       histSize,
       ranges,
       true, // uniform
       false // not accumulate
    );

    double maxVal = 0;
    minMaxLoc(histogram,
      Q_NULLPTR, // don't need min
      &maxVal,
      Q_NULLPTR, // don't need index min
      Q_NULLPTR // don't need index max
    );

    for(int i=0; i<bins; i++)
    {
        float value = histogram.at<float>(i,0);
        value = maxVal - value; // invert
        value = value / maxVal * pOutFrame->rows; // scale
        p1.y = value;
        p2.x = float(i+1) * float(outputImage.cols) / float(bins);
        rectangle(outputImage, p1, p2, Scalar(colors.at<Vec3b>(i)), CV_FILLED);
        p1.x = p2.x;
    }

    for(int i=0; i<histogram.rows; i++)
    {
      if((i==0) || (i==22) || (i==23)) // filter
        histogram.at<float>(i,0) = 255;
      else
        histogram.at<float>(i,0) = 0;
    }

    Mat hue;
    int fromto[] = {0, 0};
    hue.create(hsvImg.size(), hsvImg.depth());
    mixChannels(&hsvImg, 1, &hue, 1, fromto, 1);
    Mat backprojection;
    calcBackProject(&hue, 1, channels, histogram, backprojection, ranges);

    for(int i=0; i<hsvImg.rows; i++)
    {
      for(int j=0; j<hsvImg.cols; j++)
      {
        if(backprojection.at<uchar>(i, j))
        {
            if(hsvImg.at<Vec3b>(i,j)[0] < 60)
                hsvImg.at<Vec3b>(i,j)[0] += 120;
            else if(hsvImg.at<Vec3b>(i,j)[0] > 120)
                hsvImg.at<Vec3b>(i,j)[0] -= 60;
        }
      }
   }

   Mat imgHueShift;
   cvtColor(hsvImg, *pOutFrame, CV_HSV2BGR);
   */
}
