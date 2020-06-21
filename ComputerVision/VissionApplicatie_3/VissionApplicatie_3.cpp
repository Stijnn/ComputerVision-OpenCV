// 2.1_Camera.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    VideoCapture cap(1);

    if (!cap.isOpened())
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE);

    int lowHue      =   170;
    int highHue     =   179;
    int lowSat      =   150;
    int highSat     =   255;
    int lowVal      =   60;
    int highVal     =   255;

    createTrackbar("Low Hue", "Control", &lowHue, 179);
    createTrackbar("High Hue", "Control", &highHue, 179);

    createTrackbar("Low Saturation", "Control", &lowSat, 255);
    createTrackbar("High Saturation", "Control", &highSat, 255);

    createTrackbar("Low Value", "Control", &lowVal, 255);
    createTrackbar("High Value", "Control", &highVal, 255);

    int lastX = -1;
    int lastY = -1;

    Mat imgTmp;
    cap.read(imgTmp);

    while (true)
    {
        Mat imgOriginal;
        bool bSuccess = cap.read(imgOriginal);

        if (!bSuccess)
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

        Mat imgThresholded;

        inRange(imgHSV, Scalar(lowHue, lowSat, lowVal), Scalar(highHue, highSat, highVal), imgThresholded);

        erode   (   imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5))    );
        dilate  (   imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5))    );

        dilate  (   imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5))    );
        erode   (   imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5))    );

        Moments oMoments = moments(imgThresholded);

        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double dArea = oMoments.m00;

        if (dArea > 10000)
        {
            int posX = dM10 / dArea;
            int posY = dM01 / dArea;

            circle(imgOriginal, Point(posX, posY), 25, Scalar(1, 1, 1, 1));

            lastX = posX;
            lastY = posY;
        }

        imshow("Thresholded Image", imgThresholded);

        imshow("Original", imgOriginal);

        if (waitKey(30) == 27)
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }

    return 0;
}

//#include <opencv2/opencv.hpp>
//#include "opencv2/imgproc/imgproc.hpp" 
//#include "opencv2/highgui/highgui.hpp"
//#include <iostream>
//#include <string>
//#include <Windows.h>
//#include "blobdetectionavans.h"
//
//#include <sstream>
//#include <thread>
//
//#define WIN_1_NAME "Live Camera Feed"
//
//using namespace std;
//using namespace cv;
//
//int g_ThresholdValue = 230;
//int g_MinArea = 200, g_MaxArea = 7000;
//
//int main(int argc, char* argv[])
//{
//	VideoCapture cap(1);
//	if (!cap.isOpened())
//	{
//		cout << "Cannot open the video cam" << endl;
//		return -1;
//	}
//
//	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
//	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
//	cout << "Frame size : " << dWidth << " x " << dHeight << endl;
//
//	namedWindow(WIN_1_NAME, CV_WINDOW_AUTOSIZE);
//
//	Mat frame;
//	while (true)
//	{
//		bool bSuccess = cap.read(frame);
//		flip(frame, frame, 3);
//		if (!bSuccess)
//		{
//			cout << "Cannot read a frame from video stream" << endl;
//			break;
//		}
//
//		imshow(WIN_1_NAME, frame);
//
//		if (waitKey(1) == VK_ESCAPE)
//		{
//			cout << "esc key is pressed by user" << endl;
//			break;
//		}
//	}
//	return 0;
//}