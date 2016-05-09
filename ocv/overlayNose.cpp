// NOSE DETECTOR

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <iostream>

#include <QDebug>

using namespace cv;
using namespace std;

Mat main_overlayNose(string faceCascadeName, string noseCascadeName, string sAbsFNPicWithFace, string sAbsFNPic, int& iRecognizedFaces, int& iRecognizedNoses)
{
    iRecognizedFaces = 0;
    iRecognizedNoses = 0;

    CascadeClassifier faceCascade, noseCascade;
    Mat frame;

    if( !faceCascade.load(faceCascadeName))
    {
        cerr << "Error loading face cascade file. Exiting!" << endl;
        qDebug() << "ERR 1";
        return frame;
    }

    if( !noseCascade.load(noseCascadeName))
    {
        cerr << "Error loading nose cascade file. Exiting!" << endl;
        qDebug() << "ERR 2";
        return frame;
    }
    
    Mat noseMask = imread(sAbsFNPic);
    
    if ( !noseMask.data )
    {
        cerr << "Error loading nose mask image. Exiting!" << endl;
        qDebug() << "ERR 3";
        return frame;
    }

    // Current frame
    Mat frameGray;
    Mat frameROI, noseMaskSmall;
    Mat grayMaskSmall, grayMaskSmallThresh, grayMaskSmallThreshInv;
    Mat maskedNose, maskedFrame;
    
    char ch;
    
    // Create the capture object
    // 0 -> input arg that specifies it should take the input from the webcam
    VideoCapture cap(sAbsFNPicWithFace);
    
    // If you cannot open the webcam, stop the execution!
    if( !cap.isOpened() )
    {
        qDebug() << "ERR 4";
        return frame;
    }
    
    //create GUI windows
    //namedWindow("Frame");
    
    // Scaling factor to resize the input frames from the webcam
    float scalingFactor = 1.0;//0.75;
    
    vector<Rect> faces;
    
    // Iterate until the user presses the Esc key
    if(true)
    {
        // Capture the current frame
        cap >> frame;
        
        // Resize the frame
        resize(frame, frame, Size(), scalingFactor, scalingFactor, INTER_AREA);
        
        // Convert to grayscale
        cvtColor(frame, frameGray, CV_BGR2GRAY);
        
        // Equalize the histogram
        equalizeHist(frameGray, frameGray);
        
        // Detect faces
        faceCascade.detectMultiScale(frameGray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
        
qDebug() << "#faces:" << faces.size();
        iRecognizedFaces = faces.size();
        // Draw green circles around the nose
        for(int i = 0; i < faces.size(); i++)
        {
            Mat faceROI = frameGray(faces[i]);
            vector<Rect> noses;
            
            // In each face, detect the nose
            noseCascade.detectMultiScale(faceROI, noses, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30));
            
qDebug() << " face" << i << "  #noses:" << noses.size();

            //for(int j = 0; j < noses.size(); j++)
            {
                //Point center( faces[i].x + noses[j].x + int(noses[j].width*0.5), faces[i].y + noses[j].y + int(noses[j].height*0.5) );
                //int radius = int( (noses[j].width + noses[j].height)*0.25 );
                //circle( frame, center, radius, Scalar( 0, 255, 0 ), 4, 8, 0 );
                
                for(int j = 0; j < noses.size(); j++)
                {
                    iRecognizedNoses++;

                    Point center( faces[i].x + noses[j].x + int(noses[j].width*0.5), faces[i].y + noses[j].y + int(noses[j].height*0.5) );
                    int radius = int( (noses[j].width + noses[j].height)*0.25 );
                    //circle( frame, center, radius, Scalar( 0, 255, 0 ), 4, 8, 0 );
                    
                    // Overlay moustache
                    int w = 2.7 * noses[j].width;
                    int h = 1.3 * noses[j].height;
                    int x = faces[i].x + noses[j].x - (0.30) * w;
                    int y = faces[i].y + noses[j].y - (0.01) * h;
                    
                    frameROI = frame(Rect(x,y,w,h));
                    resize(noseMask, noseMaskSmall, Size(w,h));
                    cvtColor(noseMaskSmall, grayMaskSmall, CV_BGR2GRAY);
                    threshold(grayMaskSmall, grayMaskSmallThresh, 250, 255, CV_THRESH_BINARY_INV);
                    bitwise_not(grayMaskSmallThresh, grayMaskSmallThreshInv);
                    bitwise_and(noseMaskSmall, noseMaskSmall, maskedNose, grayMaskSmallThresh);
                    bitwise_and(frameROI, frameROI, maskedFrame, grayMaskSmallThreshInv);
                    add(maskedNose, maskedFrame, frame(Rect(x,y,w,h)));
                    break;  //one face has only one nose!!!
                }
            }
        }
        
        // Show the current frame
        //imshow("Frame", frame);
        
        // Get the keyboard input and check if it's 'Esc'
        /* 27 -> ASCII value of 'Esc' key
        ch = waitKey( 30 );
        if (ch == 27)
        {
            break;
        }
        */
    }
    
    // Release the video capture object
    cap.release();
    
    // Close all windows
    //destroyAllWindows();
    
qDebug() << "END";
    return frame;
}
