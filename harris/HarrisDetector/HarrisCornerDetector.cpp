/*
 * HarrisCornerDetector.cpp
 *
 *  Created on: 18.08.2011
 *      Author: sn
 */

#include "HarrisCornerDetector.h"

HarrisCornerDetector::HarrisCornerDetector(float dSigma, int dKernelSize,
				float gSigma, int gKernelSize, float k, float threshold)
{
	devSigma_ = dSigma;
	devKernelSize_ = dKernelSize;
	gaussSigma_ = gSigma;
	gaussKernelSize_ = gKernelSize;
	harrisK_ = k;
	threshold_ = threshold;

	input_ = 0;
	cornerStrength_ = 0;
	kernelX_ = 0;
	kernelY_ = 0;
	hcr_ = 0;
}

HarrisCornerDetector::~HarrisCornerDetector()
{
	if(input_)
		delete input_;

	if(cornerStrength_)
		delete cornerStrength_;
}

void
HarrisCornerDetector::init()
{
}

void
HarrisCornerDetector::inputImage(unsigned char *input, int width, int height)
{
}

void
HarrisCornerDetector::calculate()
{
}

void *
HarrisCornerDetector::getCorners()
{
	return 0;
}

void
HarrisCornerDetector::getCornerImage(unsigned char **corners, int *width,
				int *height)
{
}

void
HarrisCornerDetector::detectCorners(unsigned char *input, int width,
				int height, void *cornerList, unsigned char **corners)
{
}

unsigned char *
HarrisCornerDetector::extendImage()
{
	return 0;
}

void
HarrisCornerDetector::performHarris(unsigned char *extendedImg,
				float threshold, float **hcr, unsigned char **cornerStrength)
{
}
