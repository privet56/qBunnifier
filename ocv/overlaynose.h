#ifndef OVERLAYNOSE_H
#define OVERLAYNOSE_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>

cv::Mat main_overlayNose(std::string faceCascadeName, std::string noseCascadeName, std::string sAbsFNPicWithFace, std::string sAbsFNPic, int& iRecognizedFaces, int& iRecognizedNoses);
cv::Mat cartoonize(std::string fn);

#endif // OVERLAYNOSE_H
