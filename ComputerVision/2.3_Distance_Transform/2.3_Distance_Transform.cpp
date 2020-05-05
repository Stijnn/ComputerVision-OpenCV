#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;


int main(int argc, char** argv)
{
	// src = bronafbeelding
	Mat src;

	// Inladen van een afbeelding
	src = imread(argv[1]);

	// Controle of de afbeelding in orde is.
	if (!src.data)
	{
		return -1;
	}

	// De afbeelding converteren naar een grijswaarde afbeelding
	Mat gray_image;
	cvtColor(src, gray_image, CV_BGR2GRAY);

	// Grijswaarde afbeelding thresholden
	// NB: Aanpassen voor de afbeelding die je gebruikt
	Mat binaryx;
	threshold(gray_image, binaryx, 190, 1, CV_THRESH_BINARY_INV);

	namedWindow("Gray Image", WINDOW_AUTOSIZE);
	imshow("Gray Image", gray_image);
	waitKey(0);

	//namedWindow("Binary Image", WINDOW_AUTOSIZE);
	imshow("Binary Image", binaryx * 255);
	waitKey(0);

	// Distance transform
	Mat dist;
	distanceTransform(binaryx, dist, DIST_L2, 3);
	// Normaliseren over de range {0.0, 1.0}
	normalize(dist, dist, 0, 1.0, NORM_MINMAX);
	imshow("Distance Transform Image", dist);

	waitKey(0);

	return 0;
}