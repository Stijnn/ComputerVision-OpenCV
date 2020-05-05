// 2.1_Camera.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include <Windows.h>
#include "BlobDetection.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	// Open de camera met nummer 1 in lijst (red.: nr 0 was bij mij de camera in de klep van mijn laptop)  
	VideoCapture cap(1);

	// Controle of de camera wordt herkend.
	if (!cap.isOpened())
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	// Breedte en hooogte van de frames die de camera genereert ophalen. 
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	// Window maken waarin de beelden "live" getoond worden
	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE);

	// Continue loop waarin een beeld wordt opgehaald en wordt getoond in het window
	Mat frame;

	//int keyInput = waitKey(1);

	while (1)
	{

		// Lees een nieuw frame
		bool bSuccess = cap.read(frame);

		flip(frame, frame, 3);

		// Controlleer of het frame goed gelezen is.
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		if (waitKey(1) == 13) {
			// Window maken waarin de objecten getoond worden met de hoeveelheid blobs.
			cout << "Enter key is pressed by user" << endl;
			BlobDetection::blobDetect(frame);
			namedWindow("Blob", CV_WINDOW_AUTOSIZE);
		}

		// Het tonen van grijswaarde beeld
		imshow("MyVideo", frame);



		//  Wacht 30 ms op ESC-toets. Als ESC-toets is ingedrukt verlaat dan de loop
		if (waitKey(1) == 27)
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;
}
