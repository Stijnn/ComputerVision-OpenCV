// Demo Zelf Mat-objecten maken
// Typische toepassing is het maken kernels t.b.v. filtering
// Uitleg zelf Mat-objecten creeeren: http://docs.opencv.org/doc/tutorials/core/mat_the_basic_image_container/mat_the_basic_image_container.html
// Jan Oostindie, dd 9-2-2015

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat matrix1 = Mat::ones(3, 3, CV_32F);
	cout << "Eenheidsmatrix = " << endl << " " << matrix1 << endl << endl;

	Mat matrix2 = (Mat_<double>(2, 4) << 1.0, 2.3, 8.1, 6.7, -90.2, 1.0 / 9, -45.78, 90.0);
	cout << "Een matrix met reeele getallen = " << endl << " " << matrix2 << endl << endl;

	Mat Z = Mat::zeros(3, 3, CV_8UC1);
	cout << "Nul matrix = " << endl << " " << Z << endl << endl;

	// Een 2 x 2 matrix
	Mat matrix3 = (Mat_<double>(2, 2) << 1.0, 2.3, 8.1, 6.7);
	cout << "Een 2 x 2 matrix  = " << endl << " " << matrix3 << endl << endl;

	// Genormeerde matrix
	Mat matrix4 = (1 / (1.0 + 2.3 + 8.1 + 6.7)) * (Mat_<double>(2, 2) << 1.0, 2.3, 8.1, 6.7);
	cout << "Voorgaande matrix genormeerd = " << endl << " " << matrix4 << endl << endl;

	string dummy;
	getline(cin, dummy);

	return 0;
}