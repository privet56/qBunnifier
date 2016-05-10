#ifndef SEGMENTOCR_H
#define SEGMENTOCR_H

#include <opencv2/opencv.hpp>

#include <vector>
#include <fstream>

std::string identifyText(cv::Mat input, std::string language);
std::string main_ocr(std::string sAbsFN, std::string language, int& iFoundRegions);

#endif // SEGMENTOCR_H
