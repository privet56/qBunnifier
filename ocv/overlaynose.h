#ifndef OVERLAYNOSE_H
#define OVERLAYNOSE_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
using namespace std;

cv::Mat main_overlayNose(string faceCascadeName, string noseCascadeName, string sAbsFNPicWithFace, string sAbsFNPic, int& iRecognizedFaces, int& iRecognizedNoses);

#endif // OVERLAYNOSE_H
