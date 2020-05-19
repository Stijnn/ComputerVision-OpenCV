// Demo: Filters maken en toepassen. 
//       Convolution Filter - de filter2Dfunctie 
// Toepassing: b.v. Blurren, Edge detection etc. etc.
// Uitleg:http://www.aishack.in/tutorials/image-convolutions-in-opencv/
// Jan Oostindie, dd 9-2-2015

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	// met commandline argument wordt de filenaam doorgegeven
	// zie: project properties - configuration properties - debugging - command arguments
	if (argc != 2)
	{
		cout << "NB! Geef als command argument volledige padnaam van de imagefile mee" << endl;
		return -1;
	}
	else cout << "De imagefile = " << argv[1] << endl;

	// Lees de afbeelding in
	Mat image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// De afbeelding converteren naar een grijswaarde afbeelding
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	// Grijswaarde afbeelding op het scherm zetten
	cout << "Imagefile: " << argv[1] << " met succes geconverteerd naar grijswaarde beeld." << endl;
	cout << "Druk op ENTER voor vervolg ==> " << endl;
	namedWindow("Display Gray window", WINDOW_AUTOSIZE);
	imshow("Display Gray window", gray_image);

	/*** Convolutie filter toepassen ==> de functie filter2D ***/
	/*** Zie voor uitleg de hyperlink bovenaan dit programma ***/
	/*** void filter2D(
	Mat src,
	Mat dst,
	int ddepth,
	Mat kernel,
	Point anchor,
	double delta,
	int borderType);
	***/

	// Maken van een filter. Hier is dat een averaging filter
	Mat kernel1 = (Mat_<double>(3, 3) << 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
	kernel1 = 1.0 / 9 * kernel1;
	cout << " Averaging filter = " << endl << " " << kernel1 << endl << endl;

	// Uitvoeren van de filter operatie + resultaat tonen
	filter2D(gray_image, gray_image, -1, kernel1, Point(-1, -1), 0, BORDER_REPLICATE);
	imshow("Display blurred image", gray_image);
	waitKey(0);

	// Maken van een filter. In dit voorbeeld is dat een Sobel filter in x-richting
	Mat kernel2 = (Mat_<int>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
	cout << " Sobel = " << endl << " " << kernel2 << endl << endl;

	// Uitvoeren van de filter operatie + resultaat tonen
	Mat resultSobelX;
	filter2D(gray_image, resultSobelX, -1, kernel2, Point(-1, -1), 0, BORDER_REPLICATE);
	imshow("Display 1st derivative in x-direction of image", resultSobelX);
	waitKey(0);

	Mat resultSobelMagnitude;
	// Je kunt uiteraard ook filteroperaties uit OpenCV gebruiken. 
	// B.v. de Gauss-filter met sigma = 1, gevolgd door een Sobel magnitude (1ste afgeleide)  
	GaussianBlur(gray_image, gray_image, Size(3, 3), 1);
	Sobel(gray_image, resultSobelMagnitude, CV_16S, 1, 1);
	filter2D(gray_image, resultSobelMagnitude, -1, kernel2, Point(-1, -1), 0, BORDER_REPLICATE);
	imshow("Display 1st derivative of image", resultSobelMagnitude);
	waitKey(0);

	string dummy;
	getline(cin, dummy);

	return 0;
}
