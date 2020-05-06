// 2.1_Camera.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include <Windows.h>
#include "BlobDetection.h"
#include "blobdetectionavans.h"

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
			//SIMPEL BLOB DETECTION.
			//BlobDetection::blobDetect(frame);
			
			// De afbeelding converteren naar een grijswaarde afbeelding
			Mat gray_image;
			cvtColor(frame, gray_image, CV_BGR2GRAY);

			// Converteren naar grijswaarde afbeelding
			//cout << "Imagefile: " << argv[1] << " met succes geconverteerd naar grijswaarde beeld." << endl;

			//////////////////////////////////////////////////////////////////////////

			// Grijswaarde afbeelding thresholden
			Mat binaryImage;
			int thresholdvalue = 20;
			threshold(gray_image, binaryImage, thresholdvalue, 1, CV_THRESH_BINARY_INV);
			namedWindow("Original", WINDOW_AUTOSIZE);
			imshow("Original", gray_image);
			waitKey(0);

			// Alvorens bewerkingen uit te voeren op het beeld converteren we deze
			// naar een Mat object met grotere diepte (depth), t.w. 16 bits signed
			Mat binary16S;
			binaryImage.convertTo(binary16S, CV_16S);

			// functie labelBLOBs doet hetzelfde als Label Blobs in VisionLab; input is een
			// binair beeld. Output is een image waarin alle pixels van elke blob voorzien zijn van 
			// volgnummer.
			Mat labeledImage;
			cout << "Total number of BLOBs " << labelBLOBs(binary16S, labeledImage) << endl;

			// functie show16SImageStretch beeld elke image af op 0 - 255 zodat je vrijwel altijd
			// wel een redelijke view krijgt op de data.
			// (show16SImageClip laat alle pixels met een waarde tussen 0 en 255 zien. Waardes onder 0
			// worden op 0 afgebeeld, waardes boven 255 worden op 255 afgebeeld.)
			show16SImageStretch(labeledImage, "Labeled Image");

			imwrite("c:\\dump\\labeledImage.jpg", labeledImage);

			// labelBLOBsInfo: met deze functie kun je ook BLOBs labelen. De functie geeft van 
			// alle BLOBs de volgende informatie terug:
			// - coordinaten van het eerste pixel van de BLOB
			// - coordinaten van het zwaartepunt van de BLOB.
			// - area van de BLOB
			Mat labeledImage2;
			vector<Point2d*> firstpixelVec2;
			vector<Point2d*> posVec2;
			vector<int> areaVec2;
			labelBLOBsInfo(binary16S, labeledImage2, firstpixelVec2, posVec2, areaVec2);
			show16SImageStretch(labeledImage2, "Labeled Image 2");

			cout << endl << "*******************************************" << endl << endl;

			// Toon alle informatie in de console 
			cout << "Aantal gevonden BLOBs = " << firstpixelVec2.size() << endl;
			for (int i = 0; i < firstpixelVec2.size(); i++) {
				cout << "BLOB " << i + 1 << endl;
				cout << "firstpixel = (" << firstpixelVec2[i]->x << "," << firstpixelVec2[i]->y << ")" << endl;
				cout << "centre = (" << posVec2[i]->x << "," << posVec2[i]->y << ")" << endl;
				cout << "area = " << areaVec2[i] << endl;
			}

			cout << endl << "*******************************************" << endl << endl;


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
