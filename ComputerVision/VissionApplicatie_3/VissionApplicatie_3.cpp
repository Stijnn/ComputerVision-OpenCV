// 2.1_Camera.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include <Windows.h>
#include "blobdetectionavans.h"

#include <sstream>
#include <thread>

#define WIN_1_NAME "Live Camera Feed"
#define WIN_2_NAME "Live Camera Feed (Gray)"
#define WIN_3_NAME "Live Threshold Preview"
#define WIN_4_NAME "Snapshot Detection Preview"

using namespace std;
using namespace cv;

int g_ThresholdValue = 230;
int g_MinArea = 200, g_MaxArea = 7000;
bool g_SnapshotThreadRunning = true;

bool converted = false;
Mat convertedFrame;

void show_thread()
{
	while (g_SnapshotThreadRunning)
	{
		Mat					snapshotImage;
		vector<Point2d*>	firstPixelCollection;
		vector<Point2d*>	centerPointCollection;
		vector<int>			areaCollection;

		if (converted)
		{
			cout << "Total number of BLOBs " <<
				labelBLOBsInfo
				(
					convertedFrame,
					snapshotImage,
					firstPixelCollection,
					centerPointCollection,
					areaCollection,
					g_MinArea,
					g_MaxArea
				)
				<< endl;

			int totalFound = 0;
			if (areaCollection.size() > 0)
			{
				int smallest = areaCollection[0];
				int total = areaCollection[0];
				for (size_t i = 1; i < areaCollection.size(); i++)
				{
					total += areaCollection[i];
					smallest = smallest < areaCollection[i] ? smallest : areaCollection[i];
				}

				totalFound = (total / smallest);
			}

			for (size_t i = 0; i < centerPointCollection.size(); i++)
			{
				circle(snapshotImage, Point(centerPointCollection[i]->x, centerPointCollection[i]->y), 25, 1);
			}

			std::ostringstream oss;
			oss << "Blobs detected: ";
			oss << totalFound;
			std::string copyOfStr = oss.str();

			putText(snapshotImage, cv::String(copyOfStr), Point(10, 20), CV_FONT_HERSHEY_SIMPLEX, 0.75f, Scalar(1, 0, 0, 1));

			show16SImageStretch(snapshotImage, WIN_4_NAME);
		}
	}
}

int main(int argc, char* argv[])
{
	VideoCapture cap(1);
	if (!cap.isOpened())
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	std::thread blobshowThread(show_thread);

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	namedWindow(WIN_1_NAME, CV_WINDOW_AUTOSIZE);
	namedWindow(WIN_2_NAME, CV_WINDOW_AUTOSIZE);
	namedWindow(WIN_3_NAME, CV_WINDOW_AUTOSIZE);

	createTrackbar("Threshold", WIN_3_NAME, &g_ThresholdValue, 255);
	createTrackbar("Min Area", WIN_3_NAME, &g_MinArea, 20000);
	createTrackbar("Max Area", WIN_3_NAME, &g_MaxArea, 100000);

	Mat frame;
	while (g_SnapshotThreadRunning)
	{
		bool bSuccess = cap.read(frame);
		flip(frame, frame, 3);
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		imshow(WIN_1_NAME, frame);

		Mat gray_image;
		cvtColor(frame, gray_image, CV_BGR2GRAY);
		imshow(WIN_2_NAME, gray_image);

		Mat binaryImage;
		threshold(gray_image, binaryImage, g_ThresholdValue, 1, CV_THRESH_BINARY_INV);

		imshow(WIN_3_NAME, binaryImage * 255);

		Mat cvtBin;
		binaryImage.convertTo(cvtBin, CV_16S);

		convertedFrame = cvtBin;
		converted = true;

		if (waitKey(1) == VK_ESCAPE)
		{
			cout << "esc key is pressed by user" << endl;
			g_SnapshotThreadRunning = false;
			while (!blobshowThread.joinable())
			{
				cout << "cannot exit thread" << endl;
			}
			blobshowThread.join();

			break;
		}
	}
	return 0;
}