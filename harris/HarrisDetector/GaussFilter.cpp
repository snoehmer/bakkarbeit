/*
 * GaussFilter.cpp
 *
 *  Created on: 16.08.2011
 *      Author: sn
 */

#include "GaussFilter.h"
#include <cmath>
#include <cstring>


GaussFilter::GaussFilter(int kernelSize, float sigma)
{
    if(kernelSize % 2 != 0)
        this->kernelSize = kernelSize;
    else
        this->kernelSize = kernelSize - 1;

    this->sigma = sigma;

    input = 0;
    kernel = 0;
    output = 0;
}


GaussFilter::GaussFilter(unsigned char *original, int width, int height, int kernelSize, float sigma)
{
    if(kernelSize % 2 != 0)
        this->kernelSize = kernelSize;
    else
        this->kernelSize = kernelSize - 1;

    this->sigma = sigma;

    inputImage(original, width, height);

    kernel = 0;
    output = 0;
}


GaussFilter::~GaussFilter()
{
    if(kernel)
    {
        delete[] kernel;
        kernel = 0;
    }

    if(input)
    {
        delete[] input;
        input = 0;
    }

    if(output)
    {
        delete[] output;
        output = 0;
    }
}


void GaussFilter::generateKernel()
{
    int row;
    int col;
    int center = (kernelSize - 1) / 2;
    int xc2;  // = (x - center)^2
    int yc2;
    float sigma2 = sigma * sigma;

    float sum = 0;  // needed for normalization

    kernel = new float[kernelSize * kernelSize];

    // step 1: calculate Gauss function for kernel
    for(row = 0; row < kernelSize; row++)
    {
        for(col = 0; col < kernelSize; col++)
        {
            xc2 = (col - center) * (col - center);
            yc2 = (row - center) * (row - center);

            kernel[row * kernelSize + col] = 1.0f / (2 * M_PI * sigma2) * exp(((float) -(xc2 + yc2))/(2 * sigma2));

            sum += kernel[row * kernelSize + col];
        }
    }

    // step 2: normalize kernel
    for(row = 0; row < kernelSize; row++)
    {
        for(col = 0; col < kernelSize; col++)
        {
            kernel[row * kernelSize + col] = kernel[row * kernelSize + col] / sum;
        }
    }
}


void GaussFilter::inputImage(unsigned char *original, int width, int height)
{
    if(input)
    {
        delete[] input;
        input = 0;
    }

    this->width = width;
    this->height = height;

    input = new unsigned char[width * height];

    // copy original image to input buffer
    memcpy(input, original, width * height * sizeof(unsigned char));
}


void GaussFilter::calculate()
{
    unsigned char *extendedImg = 0;

    if(!kernel)
        generateKernel();

    if(!input)
    {
        output = 0;
        return;
    }

    // extend image for convolution
    extendImage(&extendedImg);

    // do real Gaussian filtering and crop image
    filter(extendedImg);
}


void GaussFilter::getFilteredImage(unsigned char **filtered, int *fWidth, int *fHeight)
{
    if(fWidth != 0)
        *fWidth = width;

    if(fHeight != 0)
        *fHeight = height;

    if(*filtered)
    {
        delete[] *filtered;
        *filtered = 0;
    }

    if(!output)
    {
        *filtered = 0;
        return;
    }

    *filtered = new unsigned char[width * height];

    memcpy(*filtered, output, width * height * sizeof(unsigned char));
}


void GaussFilter::filterImage(unsigned char *image, unsigned char **result, int width, int height)
{
    if(!kernel)  // kernel not initialized!
        generateKernel();

    inputImage(image, width, height);
    calculate();
    getFilteredImage(result);
}


void GaussFilter::extendImage(unsigned char **extendedImg)
{
    int row;
    int col;

    int offset = (kernelSize - 1) / 2;

    int extWidth = width + 2 * offset;
    int extHeight = height + 2 * offset;

    if(*extendedImg)
    {
        delete[] extendedImg;
        extendedImg = 0;
    }

    *extendedImg = new unsigned char[extWidth * extHeight];

    // step 0: copy image
    for(row = 0; row < height; row++)
        for(col = 0; col < width; col++)
            (*extendedImg)[(row + offset) * extWidth + (col + offset)] = input[row * width + col];

    // step 1a: copy upper border
    for(row = 0; row < offset; row++)
        for(col = 0; col < width; col++)
            (*extendedImg)[row * extWidth + (col + offset)] = input[0 * width + col];

    // step 1b: copy lower border
    for(row = offset + height; row < height + 2*offset; row++)
        for(col = 0; col < width; col++)
            (*extendedImg)[row * extWidth + (col + offset)] = input[(height - 1) * width + col];

    // step 1c: copy left border
    for(col = 0; col < offset; col++)
        for(row = 0; row < height; row++)
            (*extendedImg)[(row + offset) * extWidth + col] = input[row * width + 0];

    // step 1d: copy right border
    for(col = offset + width; col < width + 2*offset; col++)
        for(row = 0; row < height; row++)
            (*extendedImg)[(row + offset) * extWidth + col] = input[row * width + (width - 1)];

    // step 2a: copy upper left corner
    for(row = 0; row < offset; row++)
        for(col = 0; col < offset; col++)
            (*extendedImg)[row * extWidth + col] = input[0 * width + 0];

    // step 2b: copy upper right corner
    for(row = 0; row < offset; row++)
        for(col = offset + width; col < width + 2*offset; col++)
            (*extendedImg)[row * extWidth + col] = input[0 * width + (width - 1)];

    // step 2c: copy lower left corner
    for(row = offset + height; row < height + 2*offset; row++)
        for(col = 0; col < offset; col++)
            (*extendedImg)[row * extWidth + col] = input[(height - 1) * width + 0];

    // step 2d: copy lower right corner
    for(row = offset + height; row < height + 2*offset; row++)
        for(col = offset + width; col < width + 2*offset; col++)
            (*extendedImg)[row * extWidth + col] = input[(height - 1) * width + (width - 1)];
}


void GaussFilter::filter(unsigned char *extendedImg)
{
    int imgrow;  // current row and col in the image where the filter is calculated
    int imgcol;
    int krow;  // current row and col in the kernel for the convolution sum
    int kcol;
    int row;  // row and col of image pixel for current kernel position
    int col;

    int offset = (kernelSize - 1) / 2;

    int extWidth = width + 2 * offset;
    int extHeight = height + 2 * offset;

    float gauss;

    if(output)
    {
        delete[] output;
        output = 0;
    }

    output = new unsigned char[width * height];

    for(imgrow = offset; imgrow < extHeight - offset; imgrow++)
    {
        for(imgcol = offset; imgcol < extWidth - offset; imgcol++)
        {
            gauss = 0;

            // calculate weighted sum over kernel (convolution)
            for(krow = 0; krow < kernelSize; krow++)
            {
                for(kcol = 0; kcol < kernelSize; kcol++)
                {
                    row = imgrow + krow - offset;
                    col = imgcol + kcol - offset;

                    gauss += extendedImg[row * extWidth + col] * kernel[krow * kernelSize + kcol];
                }
            }

            output[(imgrow - offset) * width + (imgcol - offset)] = (unsigned char) round(gauss);
        }
    }
}
