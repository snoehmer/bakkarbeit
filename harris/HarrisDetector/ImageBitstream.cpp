/*
 * ImageBitstream.cpp
 *
 *  Created on: 19.08.2011
 *      Author: sn
 */

#include "ImageBitstream.h"
#include <Magick++.h>

using namespace std;
using namespace Magick;

ImageBitstream ImageBitstream::convolve(float *kernel, int kernelSize)
{
}



int ImageBitstream::getHeight()
{
	return height_;
}



unsigned char & ImageBitstream::pixel(int row, int col)
{
	if(row >= height_ || col >= width_)
		return 0;

	return bitstream_[row * width_ + col];
}



void ImageBitstream::saveImage(std::string filename)
{
	if(bitstream_)
	{
		Image img;

		img.read(width_, height_, "I", CharPixel, bitstream_);
		img.write(filename.c_str());
	}
}



void ImageBitstream::setImage(Magick::Image img)
{
	width_ = img.geometry().width();
	height_ = img.geometry().height();

	if(bitstream_)
		delete[] bitstream_;

	bitstream_ = new unsigned char[width_ * height_];

	img.write(0, 0, width_, height_, "I", CharPixel, bitstream_);
}



unsigned char *ImageBitstream::extendImage()
{
}



void ImageBitstream::setImage(std::string filename)
{
	Image img;

	img.read(filename.c_str());

	setImage(img);
}



ImageBitstream & ImageBitstream::operator =(ImageBitstream & original)
{
	width_ = original.width_;
	height_ = original.height_;

	if(bitstream_ != 0)
		delete[] bitstream_;

	bitstream_ = new unsigned char[width_ * height_];
	memcpy(bitstream_, original.bitstream_, width_ * height_ * sizeof(unsigned char));
}



ImageBitstream ImageBitstream::stretchContrast()
{
}



ImageBitstream::ImageBitstream(std::string filename)
{
	setImage(filename);
}



unsigned char *ImageBitstream::copyBitstream()
{
	unsigned char *copy;

	copy = new unsigned char[width_ * height_];
	memcpy(copy, bitstream_, width_ * height_ * sizeof(unsigned char));
}



ImageBitstream::ImageBitstream(const ImageBitstream & original)
{
	width_ = original.width_;
	height_ = original.height_;

	if(bitstream_ != 0)
		delete[] bitstream_;

	bitstream_ = new unsigned char[width_ * height_];
	memcpy(bitstream_, original.bitstream_, width_ * height_ * sizeof(unsigned char));
}



int ImageBitstream::getWidth()
{
	return width_;
}



Magick::Image ImageBitstream::getImage()
{
	Image img;

	if(bitstream_)
	{
		img.read(width_, height_, "I", CharPixel, bitstream_);
	}

	return img;
}



ImageBitstream::~ImageBitstream()
{
	if(bitstream_)
		delete[] bitstream_;
}



ImageBitstream::ImageBitstream(Magick::Image img)
{
	setImage(img);
}



unsigned char *ImageBitstream::getBitstream()
{
	return bitstream_;
}



ImageBitstream::ImageBitstream()
{
	bitstream_ = 0;
}
