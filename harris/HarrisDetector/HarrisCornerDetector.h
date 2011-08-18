/*
 * HarrisCornerDetector.h
 *
 *  Created on: 18.08.2011
 *      Author: sn
 */

#ifndef HARRISCORNERDETECTOR_H_
#define HARRISCORNERDETECTOR_H_

#include "GaussFilter.h"


class HarrisCornerDetector
{
public:

    /**
     * constructor for HarrisCornerDetector
     * @param dSigma sigma value for the derives
     * @param sKernelSize kernel size for convolution of derives
     * @param gSigma sigma value for Gauss filter
     * @param gKernelSize kernel size for convolution of Gauss filter
     * @param k parameter for Harris detector
     * @param thresh threshold for the corner strength
     */
    HarrisCornerDetector(float dSigma = 0.5f, int dKernelSize = 5, float gSigma = 0.5f, int gKernelSize = 5, float k = 0.04f, float threshold = 1.0f);

    virtual ~HarrisCornerDetector();

    /**
     * does all the initialization stuff (generating kernels etc)
     */
    void init();

    /**
     * sets the (new) input image for the corner detector
     * @param input a raw bit stream of the input image
     * @param width the width of the input image
     * @param height the height of the input image
     */
    void inputImage(unsigned char *input, int width, int height);

    /**
     * applies the edge detector to the input image
     */
    void calculate();

    /**
     * returns a list of detected corners
     * @returns list of detected corners or NULL if the detector hasnt run yet
     */
    void* getCorners();

    /**
     * returns an image of the strength of the detected corners
     * @param corners a raw bit stream of the corner image
     */
    void getCornerImage(unsigned char **corners, int *width = 0, int *height = 0);

    /**
     * performs a full Harris corner detection
     * @param input the input image
     * @param width the width of the input image
     * @param height the height of the input image
     * @param cornerList a list of the detected corners
     * @param corners a image with the corner strength
     */
    void detectCorners(unsigned char *input, int width, int height, void *cornerList, unsigned char **corners);


private:

    float devSigma_;
    int devKernelSize_;
    float gaussSigma_;
    float gaussKernelSize_;
    float harrisK_;
    float threshold_;
    unsigned char *input_;
    unsigned char *cornerStrength_;  // the thresholded and scaled corner strength
    //List of corners
    GaussFilter gaussFilter_;  // used for filtering of derives
    float *kernelX_;
    float *kernelY_;
    int kernelSize_;
    int width_;
    int height_;
    float *hcr_;  // the Harris corner response for every pixel


    /**
     * extends the image beyond its borders for the convolution
     * every pixel on the border is copied outside, so that the image is
     * extended by (kernelSize - 1)/2 pixels on each border
     * @returns the buffer containing the extended result
     */
    unsigned char* extendImage();

    /**
     * performs the convolution with both derived kernels for every pixel
     * calculates the products of derives for every pixel
     * applies a Gauss filter on the products
     * calculates the Harris corner response for every pixel
     * applies a threshold to the Harris corner response
     * the results are cropped and stored in output
     * @param extendedImg source for image to derive
     * @param threshold the threshold for the corner strength
     * @param hcr a raw bit stream of the harris corner response
     * @param cornerStrength the thresholded corner strength
     */
    void performHarris(unsigned char *extendedImg, float threshold, float **hcr, unsigned char **cornerStrength);

};

#endif /* HARRISCORNERDETECTOR_H_ */
