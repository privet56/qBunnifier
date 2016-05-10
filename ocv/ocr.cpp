#include  "opencv2/highgui.hpp"
#include  "opencv2/imgproc.hpp"
#include  "opencv2/text.hpp"

#include  <vector>
#include  <iostream>
#include <QApplication>

using namespace std;
using namespace cv;
using namespace cv::text;

vector<Mat> separateChannels(Mat& src) 
{
	vector<Mat> channels;
	//Grayscale images
	if (src.type() == CV_8U || src.type() == CV_8UC1) {
		channels.push_back(src);
		channels.push_back(255-src);
		return channels;
	}

	//Colored images
	if (src.type() == CV_8UC3) {
		computeNMChannels(src, channels);
		int size = static_cast<int>(channels.size())-1;
		for (int c = 0; c < size; c++)
			channels.push_back(255-channels[c]);
		return channels;
	}

	//Other types
	cout << "Invalid image format!" << endl;
	exit(-1);	
}

cv::Ptr<BaseOCR> initOCR(const string& ocr) 
{
    if (ocr == "hmm")
    {
		Mat transitions;
        FileStorage fs("tessdata/OCRHMM_transitions_table.xml", FileStorage::READ);
		fs["transition_probabilities"] >> transitions;
		fs.release();

        QString qs(qApp->applicationDirPath()+"/tessdata/OCRHMM_knn_model_data.xml.gz");
        string s = qs.toStdString();

		return OCRHMMDecoder::create(
            loadOCRHMMClassifierNM(s),        //Trained models
            string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789äüöß"),   //Vocabulary
            transitions,                                                            //Trained transition probabilities
            Mat::eye(62,62,CV_64FC1));                                              //Emission probabilities. Identity matrix.
    }
    else if (ocr == "tesseract" || ocr == "tess")
    {
        return OCRTesseract::create(nullptr, "eng");
	}

	throw string("Invalid OCR engine: ") + ocr;	
}

Mat deskewAndCrop(Mat input, const RotatedRect& box)
{
	double angle = box.angle;	
	Size2f size = box.size;

	//Adjust the box angle
   if (angle < -45.0) 
	{
        angle += 90.0;
        std::swap(size.width, size.height);		
	}
	
	//Rotate the text according to the angle
	Mat transform = getRotationMatrix2D(box.center, angle, 1.0);
	Mat rotated;
	warpAffine(input, rotated, transform, input.size(), INTER_CUBIC);

	//Crop the result
	Mat cropped;
	getRectSubPix(rotated, size, box.center, cropped);
	copyMakeBorder(cropped,cropped,10,10,10,10,BORDER_CONSTANT,Scalar(0));
	return cropped;
}

Mat drawER(const vector<Mat> &channels, const vector<vector<ERStat> > &regions, const vector<Vec2i>& group, const Rect& rect)
{
	Mat out = Mat::zeros(channels[0].rows+2, channels[0].cols+2, CV_8UC1);

	int flags = 4					//4 neighbors
		+ (255 << 8)				//paint mask in white (255)
		+ FLOODFILL_FIXED_RANGE		//fixed range
		+ FLOODFILL_MASK_ONLY;		//Paint just the mask
	
    for (int g=0; g < group.size(); g++)
    {
		int idx = group[g][0];		
        ERStat er = regions[idx][group[g][1]];

		//Ignore root region
        if (er.parent == NULL) 
			continue;

		//Transform the linear pixel value to row and col
		int px = er.pixel % channels[idx].cols;
		int py = er.pixel / channels[idx].cols;
		
		//Create the point and adds it to the list.
		Point p(px, py);		

		//Draw the extremal region
        floodFill(
			channels[idx], out,				//Image and mask
			p, Scalar(255),					//Seed and color
			nullptr,						//No rect
			Scalar(er.level),Scalar(0),		//LoDiff and upDiff
			flags							//Flags
		);		
    }
	
	//Crop just the text area and find it's points
	out = out(rect);

	vector<Point> points;	
	findNonZero(out, points);
	//Use deskew and crop to crop it perfectly
	return deskewAndCrop(out, minAreaRect(points));
}

string main_ocr2(string sAbsFN, std::string language, int& iFoundRegions)
{
    auto input = imread(sAbsFN.c_str());

	//Convert the input image to grayscale.
	//Just do Mat processed = input; to work with colors.
	Mat processed;
	cvtColor(input, processed, CV_RGB2GRAY);

	auto channels = separateChannels(processed);
    vector<vector<ERStat> > regions(channels.size());

    // Create ERFilter objects with the 1st and 2nd stage classifiers
    if(true)
    {
        QString qs1 = qApp->applicationDirPath()+"/tessdata/trained_classifierNM1.xml";
        QString qs2 = qApp->applicationDirPath()+"/tessdata/trained_classifierNM2.xml";
        string s1 = qs1.toStdString();
        string s2 = qs2.toStdString();

        auto filter1 = createERFilterNM1(loadClassifierNM1(s1),15,0.00015f,0.13f,0.2f,true,0.1f);
        auto filter2 = createERFilterNM2(loadClassifierNM2(s2),0.5);

        //Extract text regions using Newmann & Matas algorithm

        for (int c=0; c < channels.size(); c++)
        {
            filter1->run(channels[c], regions[c]);
            filter2->run(channels[c], regions[c]);
        }
        filter1.release();
        filter2.release();
    }
    //Separate character groups from regions
    vector< vector<Vec2i> > groups;
    vector<Rect> groupRects;
    if(true)
    {
        erGrouping(input, channels, regions, groups, groupRects, ERGROUPING_ORIENTATION_HORIZ);
    }
    else
    {
        QString qstrained_classifier_erGrouping(qApp->applicationDirPath()+"/tessdata/trained_classifier_erGrouping.xml");
        string trained_classifier_erGrouping = qstrained_classifier_erGrouping.toStdString();
        erGrouping(input, channels, regions, groups, groupRects, ERGROUPING_ORIENTATION_ANY, trained_classifier_erGrouping, 0.5);
    }

    string sre;
    iFoundRegions = groups.size();
    // text detection
	auto ocr = initOCR("tesseract");
	for (int i = 0; i < groups.size(); i++) 
	{
		 Mat wordImage = drawER(channels, regions, groups[i], groupRects[i]);
		 string word;
		 ocr->run(wordImage, word);
         sre += word;
	}
    return sre;
}

