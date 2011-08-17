/*
 * GaussianFilter.cpp
 *
 *  Created on: 16.08.2011
 *      Author: sn
 */

#include "GaussianFilter.h"
#include <cmath>


GaussianFilter::GaussianFilter(int *original, float sigma, int kernelSize, int width, int height)
{
    if(kernelSize % 2 != 0)
        this->kernelSize = kernelSize;
    else
        this->kernelSize = kernelSize - 1;

    this->sigma = sigma;
    this->width = width;
    this->height = height;

    input = new int[width * height];
    output = new int[width * height];
    kernel = new float[kernelSize * kernelSize];

    // copy original image to input buffer
    memcpy(input, original, width * height * sizeof(int));
}


GaussianFilter::~GaussianFilter()
{
    delete[] kernel;
    delete[] input;
    delete[] output;
}


void GaussianFilter::generateKernel()
{
    int x;
    int y;
    int center = (kernelSize - 1) / 2;
    int xc2;  // = (x - center)^2
    int yc2;
    float sigma2 = sigma * sigma;

    float sum = 0;  // needed for normalization

    // step 1: calculate Gauss function for kernel
    for(x = 0; x < kernelSize; x++)
    {
        for(y = 0; y < kernelSize; y++)
        {
            xc2 = (x - center) * (x - center);
            yc2 = (y - center) * (y - center);

            kernel[x * kernelSize + y] = 1.0f / (2 * M_PI * sigma2) * exp(((float) -(xc2 + yc2))/(2 * sigma2));

            sum += kernel[x * kernelSize + y];
        }
    }

    // step 2: normalize kernel
    for(x = 0; x < kernelSize; x++)
    {
        for(y = 0; y < kernelSize; y++)
        {
            kernel[x * kernelSize + y] = kernel[x * kernelSize + y] / sum;
        }
    }
}


void GaussianFilter::calculateGF()
{
    int *extended;
}


void GaussianFilter::getFilteredImage(int **filtered, int *fWidth, int *fHeight)
{
    *fWidth = width;
    *fHeight = height;

    *filtered = new int[width * height];

    memcpy(*filtered, output, width * height * sizeof(int));
}


void GaussianFilter::extendImage()
{
    int extWidth = width + (kernelSize - 1);
    int extHeight = height + (kernelSize - 1);


}


void GaussianFilter::cropImage()
{
}
