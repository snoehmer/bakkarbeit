/*
 * GaussianFilter.h
 *
 *  Created on: 16.08.2011
 *      Author: sn
 */

#ifndef GAUSSIANFILTER_H_
#define GAUSSIANFILTER_H_

class GaussianFilter
{
public:

    /**
     * constructor for the Gaussian filter
     * @param original the original image
     * @param sigma the sigma parameter of the Gauss filter
     * @param kernelSize size of the filter kernel (should be odd)
     * @param width the width of the image
     * @param height the height of the image
     */
    GaussianFilter(int *original, float sigma, int kernelSize, int width, int height);

    virtual ~GaussianFilter();

    /**
     * generates the Gaussian kernel for the specified kernel size and sigma
     */
    void generateKernel();

    /**
     * performs the Gaussian filter and returns the filtered image
     * first extends the image for convolution, then performs convolution
     * with Gauss kernel, then crops the unneeded borders from the image
     * stores the result internally until the user calls getFilteredImage!
     */
    void calculateGF();

    /**
     * copies the filtered image to a newly generated array of the right size
     * @param filtered the filtered image (memory is allocated IN THIS FUNCTION!)
     * @param width the width of the filtered image
     * @param height the height of the filtered image
     */
    void getFilteredImage(int **filtered, int *fWidth, int *fHeight);


private:
    int *input;
    int *output;
    float *kernel;
    float sigma;
    int kernelSize;
    int height;
    int width;

    /**
     * extends the image beyond its borders for the convolution
     * every pixel on the border is copied outside, so that the image is
     * extended by (kernelSize - 1)/2 pixels on each border
     */
    void extendImage();

    /**
     * crops the unneeded borders from the image after convolution
     */
    void cropImage();
};

#endif /* GAUSSIANFILTER_H_ */
