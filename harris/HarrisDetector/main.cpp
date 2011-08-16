/*
 * main.cpp
 *
 *  Created on: 16.08.2011
 *      Author: sn
 */

#include <Magick++.h>

using namespace std;
using namespace Magick;

int main(int argc, char **argv)
{
    InitializeMagick(NULL);

    Image img("100x100", "white");
    img.pixelColor(49, 49, "red");
    img.write("red_pixel.png");

    return 0;
}
