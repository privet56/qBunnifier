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

        //BlackMakerThread::make(&inFrame, &outFrame);

        inFrame.copyTo(outFrame);

        BlackMakerThread::hsvHist(inFrame, outFrame);


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

void BlackMakerThread::make()
{
    QString sFN("c:\\temp\\__\\in.png");
    using namespace cv;
    Mat inFrame = imread(sFN.toStdString());
    Mat outFrame;
    inFrame.copyTo(outFrame);

    BlackMakerThread::hsvHist(inFrame, outFrame);

    imshow(tr("Output Image").toStdString(), outFrame);
}

void BlackMakerThread::rgbHist(const cv::Mat &inputImage, cv::Mat &outputImage)
{
    int binsSpinvalue = 33;
    int heightSpinvalue = 333;
    int widthSpinvalue = 333;

    using namespace cv;
    using namespace std;

    int channels[] = {0};
    int histSize[] = {binsSpinvalue}; // number of bins

    float range[] = {0,255}; // range of colors
    const float* ranges[] = { range };

    Mat histograms[3];

    vector<Mat> planes;
    split(inputImage, planes);

    double maxVal[3] = {0,0,0};

    for(int i=0; i<3; i++)
    {
        calcHist(&planes[i],
                 1, // number of images
                 channels,
                 Mat(), // no masks, an empty Mat
                 histograms[i],
                 1, // dimensionality
                 histSize,
                 ranges);

        minMaxLoc(histograms[i],
                  Q_NULLPTR, // don't need min
                  &maxVal[i],
                  Q_NULLPTR, // don't need index min
                  Q_NULLPTR // don't need index max
                  );
    }

    outputImage.create(heightSpinvalue, // any image width
                       widthSpinvalue, // any image height
                       CV_8UC(3));

    outputImage = Scalar::all(0); // empty black image

    Point p1[3], p2[3];
    for(int i=0; i<binsSpinvalue; i++)
    {
        for(int j=0; j<3; j++)
        {
            float value = histograms[j].at<float>(i,0);
            value = maxVal[j] - value; // invert
            value = value / maxVal[j] * outputImage.rows;
            line(outputImage,
                 p1[j],
                 Point(p1[j].x,value),
                 Scalar(j==0 ? 255:0,
                        j==1 ? 255:0,
                        j==2 ? 255:0),
                 2);
            p1[j].y = p2[j].y = value;
            p2[j].x = float(i+1) * float(outputImage.cols) / float(binsSpinvalue);
            line(outputImage,
                 p1[j], p2[j],
                 Scalar(j==0 ? 255:0,
                        j==1 ? 255:0,
                        j==2 ? 255:0),
                 2);
            p1[j].x = p2[j].x;
        }
    }
}

void BlackMakerThread::hsvHist(const cv::Mat &inputImage, cv::Mat &outputImage)
{
    using namespace cv;
    int binsSpinvalue   = 24*5;
    int heightSpinvalue = 333;
    int widthSpinvalue  = 333;


    Mat hsv; Mat hue;
    cvtColor( inputImage, hsv, COLOR_BGR2HSV );
    hue.create( hsv.size(), hsv.depth() );
    int ch[] = { 0, 0 };
    mixChannels( &hsv, 1, &hue, 1, ch, 1 );

    MatND hist;
    int histSize = MAX( binsSpinvalue, 2 );
    float hue_range[] = { 0, 180 };
    const float* ranges = { hue_range };
    calcHist( &hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false );
    normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );
    MatND backproj;
    calcBackProject( &hue, 1, 0, hist, backproj, &ranges, 1, true );
    /*imshow( "BackProj", backproj );
    {
        Mat colors(1, binsSpinvalue, CV_8UC3);
        {
            for(int i=0; i<binsSpinvalue; i++)
            {
                colors.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>((i+1)*180.0/binsSpinvalue), 255, 255);
            }
            cvtColor(colors, colors, COLOR_HSV2BGR);
        }

        int w = 400; int h = 400;
        int bin_w = cvRound( (double) w / histSize );
        Mat histImg = Mat::zeros( w, h, CV_8UC3 );
        for( int i = 0; i < binsSpinvalue; i ++ )
        {
            rectangle( histImg, Point( i*bin_w, h ), Point( (i+1)*bin_w, h - cvRound( hist.at<float>(i)*h/255.0 ) ), Scalar(colors.at<Vec3b>(i)), -1);
        }
        //imshow( "Histogram", histImg );
    }*/

    Mat inputImagehsvImg;
    cvtColor(inputImage, inputImagehsvImg, CV_BGR2HSV);
    inputImagehsvImg.copyTo(outputImage);

    //in skin color:(white): 015, 160, 160 or  015, 130, 150
    //out skin color(black): 015, 080, 080 or  015  100, 100

    for(int i=0; i<inputImage.rows; i++)
    {
      for(int j=0; j<inputImage.cols; j++)
      {
        if(backproj.at<uchar>(i, j))
        {
            if( inputImagehsvImg.at<Vec3b>(i,j)[0] < 60)
            {
                //outputImage.at<Vec3b>(i,j)[0] += 120;  //120 makes lila
                //outputImage.at<Vec3b>(i,j)[0] += 50;   //50 makes green
                //outputImage.at<Vec3b>(i,j)[0] -= 120;  //-120 makes lila
                //outputImage.at<Vec3b>(i,j)[0] += 20;     //20 makes asian

                //brown: hsv: 01, 98, 35-45(less:darker!)
                //brown: hsv: 25, 99, 40-60(less:darker!)

                if(false)
                {   //makes black
                    outputImage.at<Vec3b>(i,j)[1] -= 50;
                    outputImage.at<Vec3b>(i,j)[2] -= 50;
                }
                else
                {   //makes asian/chinese
                    outputImage.at<Vec3b>(i,j)[0] += 20;
                }

            }
            //else if(inputImagehsvImg.at<Vec3b>(i,j)[0] > 120)
              //  outputImage.at<Vec3b>(i,j)[0] -= 60;
        }
      }
   }

   cvtColor(outputImage, outputImage, CV_HSV2BGR);

   //imshow("Result", outputImage);

    /*
    using namespace cv;
    Mat hsvImg;
    cvtColor(inputImage, hsvImg, CV_BGR2HSV);

    int channels[] = {1}; // only the first channel
    int histSize[] = {binsSpinvalue}; // number of bins

    float rangeHue[] = {0,179}; // range of Hue channel
    const float* ranges[] = { rangeHue };

    Mat histogram;

    calcHist(&hsvImg,
             1, // number of images
             channels,
             Mat(), // no masks, an empty Mat
             histogram,
             1, // dimensionality
             histSize,
             ranges);

    /*
    for(int i=0; i<histogram.rows; i++)
    {
        if(i < 40) // threshold
            histogram.at<float>(i,0) = 255;
        else
            histogram.at<float>(i,0) = 0;
    }* /

    for(int i=0; i<histogram.rows; i++)
    {
      if((i==0) || (i==22) || (i==23)) // filter
      if((i>1) && (i<25))
        histogram.at<float>(i,0) = 255;
      else
        histogram.at<float>(i,0) = 0;
    }

    Mat backprojection;
    if(true)
    {
        Mat hue;
        int fromto[] = {0, 0};
        hue.create(inputImage.size(), inputImage.depth());
        mixChannels(&inputImage, 1, &hue, 1, fromto, 1);
        Mat backprojection;
        calcBackProject(&hue,1,channels,histogram,backprojection,ranges);

        for(int i=0; i<inputImage.rows; i++)
            {
              for(int j=0; j<inputImage.cols; j++)
              {
                if(backprojection.at<uchar>(i, j))
                {
                    if( inputImage.at<Vec3b>(i,j)[0] < 60)
                        outputImage.at<Vec3b>(i,j)[0] += 120;
                    else if(inputImage.at<Vec3b>(i,j)[0] > 120)
                        outputImage.at<Vec3b>(i,j)[0] -= 60;
                }
              }
           }

           Mat imgHueShift;
           cvtColor(hsvImg, imgHueShift, CV_HSV2BGR);
    }

    bool showHistogramImage = true;
    if(showHistogramImage)
    {
        Mat histimage;
        normalize(histogram, histogram, 0, 255, NORM_MINMAX);

        double maxVal = 0;
        minMaxLoc(histogram,
                  Q_NULLPTR, // don't need min
                  &maxVal,
                  Q_NULLPTR, // don't need index min
                  Q_NULLPTR // don't need index max
                  );

        histimage.create(heightSpinvalue, // any image width
                           widthSpinvalue, // any image height
                           CV_8UC(3));

        histimage = Scalar::all(0); // empty black image

        Mat colors(1, binsSpinvalue, CV_8UC3);
        for(int i=0; i<binsSpinvalue; i++)
        {
            colors.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>((i+1)*180.0/binsSpinvalue), 255, 255);
        }
        cvtColor(colors, colors, COLOR_HSV2BGR);

        Point p1(0,0), p2(0,histimage.rows-1);
        for(int i=0; i<binsSpinvalue; i++)
        {
            float value = histogram.at<float>(i,0);
            value = maxVal - value; // invert
            value = value / maxVal * histimage.rows; // scale
            p1.y = value;
            p2.x = float(i+1) * float(histimage.cols) / float(binsSpinvalue);
            rectangle(histimage,p1,p2,Scalar(colors.at<Vec3b>(i)),CV_FILLED);
            p1.x = p2.x;
        }
        imshow("histogram", histimage);
    }
    */
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
