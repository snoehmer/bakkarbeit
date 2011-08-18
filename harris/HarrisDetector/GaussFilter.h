/*
 * GaussFilter.h
 *
 *  Created on: 16.08.2011
 *      Author: sn
 */

#ifndef GAUSSFILTER_H_
#define GAUSSFILTER_H_

class GaussFilter
{
public:

    /**
     * constructor for the Gaussian filter
     * @param kernelSize size of the filter kernel (should be odd)
     * @param sigma the sigma parameter of the Gauss filter
     */
    GaussFilter(int kernelSize = 3, float sigma = 0.5f);

    /**
     * constructor for the Gaussian filter
     * @param original the original image
     * @param width the width of the image
     * @param height the height of the image
     * @param kernelSize size of the filter kernel (should be odd)
     * @param sigma the sigma parameter of the Gauss filter
     */
    GaussFilter(unsigned char *original, int width, int height, int kernelSize = 3, float sigma = 0.2f);

    virtual ~GaussFilter();

    /**
     * generates the Gaussian kernel for the specified kernel size and sigma
     * has to be called only once!
     */
    void generateKernel();

    /**
     * reads a new image for filtering
     * the old image is deleted
     * @param image the new image as raw array
     * @param width the width of the image
     * @param height the height of the image
     */
    void inputImage(unsigned char *original, int width, int height);

    /**
     * applies the Gaussian filter and returns the filtered image
     * first extends the image for convolution, then performs convolution
     * with Gauss kernel, then crops the unneeded borders from the image
     * stores the result internally until the user calls getFilteredImage!
     */
    void calculate();

    /**
     * copies the filtered image to a newly generated array of the right size
     * @param filtered the filtered image (memory is allocated IN THIS FUNCTION!)
     * @param width the width of the filtered image
     * @param height the height of the filtered image
     */
    void getFilteredImage(unsigned char **filtered, int *fWidth = 0, int *fHeight = 0);

    /**
     * performs a full gaussian filtering on the image and generates the
     * resulting image
     * this function just calls all the other functions (except generateKernel!)
     * @param image the image to be filtered
     * @param output the buffer for the resulting image (is newly allocated)
     * @param width the width of the image
     * @param height the height of the image
     */
    void filterImage(unsigned char *image, unsigned char **result, int width, int height);


private:
    unsigned char *input;
    unsigned char *output;
    float *kernel;
    float sigma;
    int kernelSize;
    int height;
    int width;

    /**
     * extends the image beyond its borders for the convolution
     * every pixel on the border is copied outside, so that the image is
     * extended by (kernelSize - 1)/2 pixels on each border
     * @param extendedImg the buffer to store the result
     */
    void extendImage(unsigned char **extendedImg);

    /**
     * performs the gaussian filtering (convolution with filter kernel)
     * the results are cropped and stored in output
     * @param extendedImg source for filtered image
     */
    void filter(unsigned char *extendedImg);
};

#endif /* GAUSSFILTER_H_ */
