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
        kernelSize_ = kernelSize;
    else
        kernelSize_ = kernelSize - 1;

    sigma_ = sigma;

    input_ = 0;
    kernel_ = 0;
    output_ = 0;
}


GaussFilter::GaussFilter(unsigned char *original, int width, int height, int kernelSize, float sigma)
{
    if(kernelSize % 2 != 0)
        kernelSize_ = kernelSize;
    else
        kernelSize_ = kernelSize - 1;

    sigma_ = sigma;

    inputImage(original, width, height);

    kernel_ = 0;
    output_ = 0;
}


GaussFilter::~GaussFilter()
{
    if(kernel_)
    {
        delete[] kernel_;
        kernel_ = 0;
    }

    if(input_)
    {
        delete[] input_;
        input_ = 0;
    }

    if(output_)
    {
        delete[] output_;
        output_ = 0;
    }
}


void GaussFilter::generateKernel()
{
    int row;
    int col;
    int center = (kernelSize_ - 1) / 2;
    int xc2;  // = (x - center)^2
    int yc2;
    float sigma2 = sigma_ * sigma_;

    float sum = 0;  // needed for normalization

    kernel_ = new float[kernelSize_ * kernelSize_];

    // step 1: calculate Gauss function for kernel
    for(row = 0; row < kernelSize_; row++)
    {
        for(col = 0; col < kernelSize_; col++)
        {
            xc2 = (col - center) * (col - center);
            yc2 = (row - center) * (row - center);

            kernel_[row * kernelSize_ + col] = 1.0f / (2 * M_PI * sigma2) * exp(((float) -(xc2 + yc2))/(2 * sigma2));

            sum += kernel_[row * kernelSize_ + col];
        }
    }

    // step 2: normalize kernel
    for(row = 0; row < kernelSize_; row++)
    {
        for(col = 0; col < kernelSize_; col++)
        {
            kernel_[row * kernelSize_ + col] = kernel_[row * kernelSize_ + col] / sum;
        }
    }
}


void GaussFilter::inputImage(unsigned char *original, int width, int height)
{
    if(input_)
    {
        delete[] input_;
        input_ = 0;
    }

    width_ = width;
    height_ = height;

    input_ = new unsigned char[width_ * height_];

    // copy original image to input buffer
    memcpy(input_, original, width_ * height_ * sizeof(unsigned char));
}


void GaussFilter::calculate()
{
    unsigned char *extendedImg = 0;

    if(!kernel_)
        generateKernel();

    if(!input_)
    {
        output_ = 0;
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
        *fWidth = width_;

    if(fHeight != 0)
        *fHeight = height_;

    if(!output_)
    {
        *filtered = 0;
        return;
    }

    *filtered = new unsigned char[width_ * height_];

    memcpy(*filtered, output_, width_ * height_ * sizeof(unsigned char));
}


void GaussFilter::filterImage(unsigned char *image, unsigned char **result, int width, int height)
{
    if(!kernel_)  // kernel not initialized!
        generateKernel();

    inputImage(image, width, height);
    calculate();
    getFilteredImage(result);
}


void GaussFilter::extendImage(unsigned char **extendedImg)
{
    int row;
    int col;

    int offset = (kernelSize_ - 1) / 2;

    int extWidth = width_ + 2 * offset;
    int extHeight = height_ + 2 * offset;

    if(*extendedImg)
    {
        delete[] extendedImg;
        extendedImg = 0;
    }

    *extendedImg = new unsigned char[extWidth * extHeight];

    // step 0: copy image
    for(row = 0; row < height_; row++)
        for(col = 0; col < width_; col++)
            (*extendedImg)[(row + offset) * extWidth + (col + offset)] = input_[row * width_ + col];

    // step 1a: copy upper border
    for(row = 0; row < offset; row++)
        for(col = 0; col < width_; col++)
            (*extendedImg)[row * extWidth + (col + offset)] = input_[0 * width_ + col];

    // step 1b: copy lower border
    for(row = offset + height_; row < height_ + 2*offset; row++)
        for(col = 0; col < width_; col++)
            (*extendedImg)[row * extWidth + (col + offset)] = input_[(height_ - 1) * width_ + col];

    // step 1c: copy left border
    for(col = 0; col < offset; col++)
        for(row = 0; row < height_; row++)
            (*extendedImg)[(row + offset) * extWidth + col] = input_[row * width_ + 0];

    // step 1d: copy right border
    for(col = offset + width_; col < width_ + 2*offset; col++)
        for(row = 0; row < height_; row++)
            (*extendedImg)[(row + offset) * extWidth + col] = input_[row * width_ + (width_ - 1)];

    // step 2a: copy upper left corner
    for(row = 0; row < offset; row++)
        for(col = 0; col < offset; col++)
            (*extendedImg)[row * extWidth + col] = input_[0 * width_ + 0];

    // step 2b: copy upper right corner
    for(row = 0; row < offset; row++)
        for(col = offset + width_; col < width_ + 2*offset; col++)
            (*extendedImg)[row * extWidth + col] = input_[0 * width_ + (width_ - 1)];

    // step 2c: copy lower left corner
    for(row = offset + height_; row < height_ + 2*offset; row++)
        for(col = 0; col < offset; col++)
            (*extendedImg)[row * extWidth + col] = input_[(height_ - 1) * width_ + 0];

    // step 2d: copy lower right corner
    for(row = offset + height_; row < height_ + 2*offset; row++)
        for(col = offset + width_; col < width_ + 2*offset; col++)
            (*extendedImg)[row * extWidth + col] = input_[(height_ - 1) * width_ + (width_ - 1)];
}


void GaussFilter::filter(unsigned char *extendedImg)
{
    int imgrow;  // current row and col in the image where the filter is calculated
    int imgcol;
    int krow;  // current row and col in the kernel for the convolution sum
    int kcol;
    int row;  // row and col of image pixel for current kernel position
    int col;

    int offset = (kernelSize_ - 1) / 2;

    int extWidth = width_ + 2 * offset;
    int extHeight = height_ + 2 * offset;

    float gauss;

    if(output_)
    {
        delete[] output_;
        output_ = 0;
    }

    output_ = new unsigned char[width_ * height_];

    for(imgrow = offset; imgrow < extHeight - offset; imgrow++)
    {
        for(imgcol = offset; imgcol < extWidth - offset; imgcol++)
        {
            gauss = 0;

            // calculate weighted sum over kernel (convolution)
            for(krow = 0; krow < kernelSize_; krow++)
            {
                for(kcol = 0; kcol < kernelSize_; kcol++)
                {
                    row = imgrow + krow - offset;
                    col = imgcol + kcol - offset;

                    gauss += extendedImg[row * extWidth + col] * kernel_[krow * kernelSize_ + kcol];
                }
            }

            output_[(imgrow - offset) * width_ + (imgcol - offset)] = (unsigned char) round(gauss);
        }
    }
}
