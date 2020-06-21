// 2.1_Camera.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

//Includes for the counting of fingers
#include "BackgroundRemover.h"
#include "FaceDetector.h"
#include "SkinDetector.h"
#include "FingerCount.h"

#define ORIGINAL_FRAME_WIN "Live Camera Feed"
#define FOREGROUND_WIN "Foreground Camera Feed"
#define MASK_HAND_WIN "Hand Mask Camera Feed"
#define HAND_DETECT_WIN "Hand Detection Camera Feed"


using namespace cv;
using namespace std;

Mat frame, frameOut, handMask, foreground, fingerCountDebug, tempImg;

BackgroundRemover backgroundRemover;
FaceDetector faceDetector;
SkinDetector skinDetector;
FingerCount fingerCount_Left, fingerCount_Right;


int main(int argc, char** argv)
{
    VideoCapture cap(0); //capture the video from webcam
    
    //This opens the config menu for the selected capture device. It allows you to take controll of all lighting settings to better the detection of the fingers!
    cap.set(CV_CAP_PROP_SETTINGS, 1);

    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    cout << "Frame size : " << dWidth << " x " << dHeight << endl;

    namedWindow(ORIGINAL_FRAME_WIN, CV_WINDOW_AUTOSIZE);
    namedWindow(FOREGROUND_WIN, CV_WINDOW_AUTOSIZE);
    namedWindow(MASK_HAND_WIN, CV_WINDOW_AUTOSIZE);
    namedWindow(HAND_DETECT_WIN, CV_WINDOW_AUTOSIZE);

    while (true)
    {
        bool bSuccess = cap.read(frame); // read a new frame from video
        cap >> frame;
        frameOut = frame.clone();

        skinDetector.drawSkinColorSampler(frameOut);
        foreground = backgroundRemover.getForeground(frame);
        faceDetector.removeFaces(frame, foreground);
        handMask = skinDetector.getSkinMask(foreground);
        fingerCountDebug = fingerCount_Left.findFingersCount(handMask, frameOut);


        //Creating all Windows for the appliction
        imshow(ORIGINAL_FRAME_WIN, frameOut);
        imshow(FOREGROUND_WIN, foreground);
        imshow(MASK_HAND_WIN, handMask);
        imshow(HAND_DETECT_WIN, fingerCountDebug);

        int key = waitKey(1);

        // if There is no succes at getting frames program stops.
        if (!bSuccess)
        {
            cout << "Cant get find frames to update" << endl;
            break;
        }

        // multiple keyboard keys for entering the detection stage of the program
        if (key == 27)// esc
        {
            break;
        }           
        else if (key == 109) // Keyboard Key:  M / Background Mask
            backgroundRemover.calibrate(frame);
        else if (key == 100) // Keyboard Key: D
            skinDetector.calibrate(frame);
    }

    return 0;
}
