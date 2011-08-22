/*
 * main.cpp
 *
 *  Created on: 16.08.2011
 *      Author: sn
 */

#include <Magick++.h>
#include "GaussFilter.h"
#include "ImageBitstream.h"

using namespace std;
using namespace Magick;

int main(int argc, char **argv)
{
    InitializeMagick(0);

    // generate demo image
    Image demoImage("11x11", "white");
    demoImage.pixelColor(5, 5, "red");
    demoImage.write("output/red_pixel.png");

    ImageBitstream inputImg("output/red_pixel.png");
    ImageBitstream filteredImg;

    // perform harris corner detection
    // step 1: apply a Gauss filter to the image
    GaussFilter gf(5, 1.0f);

    filteredImg = gf.filterImage(inputImg);

    // step 2: apply Harris operator

    // step 3: ...


    // convert raw pixel data back to image
    filteredImg.saveImage("output/result.png");

    return 0;
}
