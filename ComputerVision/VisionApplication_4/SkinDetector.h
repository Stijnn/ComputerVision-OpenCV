#pragma once

#include<opencv\cv.h>

using namespace cv;
using namespace std;

class SkinDetector {
public:
	SkinDetector(void);

	void drawSkinColorSampler(Mat input);
	void calibrate(Mat input);
	Mat getSkinMask(Mat input);

private:
	int hLowThreshold = 0;
	int hHighThreshold = 0;
	int sLowThreshold = 0;
	int sHighThreshold = 0;
	int vLowThreshold = 0;
	int vHighThreshold = 0;

	bool calibrated = false;

	Rect skinColorSamplerRectangleLeft1, skinColorSamplerRectangleLeft2, skinColorSamplerRectangleRight1, skinColorSamplerRectangleRight2;

	void calculateThresholds(Mat sample1, Mat sample2);
	void performOpening(Mat binaryImage, int structuralElementShapde, Point structuralElementSize);
};