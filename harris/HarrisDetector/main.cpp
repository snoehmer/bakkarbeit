/*
 * main.cpp
 *
 *  Created on: 16.08.2011
 *      Author: sn
 */

#include <Magick++.h>
#include "GaussFilter.h"

using namespace std;
using namespace Magick;

int main(int argc, char **argv)
{
    InitializeMagick(NULL);

    // generate demo image
    Image demoImage("11x11", "white");
    demoImage.pixelColor(5, 5, "red");
    demoImage.write("output/red_pixel.png");

    // load image
    Image img("output/red_pixel.png");
    Geometry imgGeom = img.size();
    unsigned int imgWidth = img.size().width();
    unsigned int imgHeight = img.size().height();

    // get raw pixel grayscale data from image
    unsigned char *imgPixels = new unsigned char[imgWidth * imgHeight];  // buffer for grayscale raw image

    img.write(0, 0, imgWidth, imgHeight, "I", CharPixel, imgPixels);

    // perform harris corner detection
    // step 1: apply a Gauss filter to the image
    unsigned char *filteredPixels = 0;
    GaussFilter gf(5, 1.0f);

    gf.generateKernel();
    gf.filterImage(imgPixels, &filteredPixels, imgWidth, imgHeight);

    // step 2: apply Harris operator

    // step 3: ...


    // convert raw pixel data back to image
    Image outputImg;
    outputImg.read(imgWidth, imgHeight, "I", CharPixel, filteredPixels);
    outputImg.write("output/result.png");

    return 0;
}
