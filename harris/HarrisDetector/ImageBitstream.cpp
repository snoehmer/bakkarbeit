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
	unsigned char *extendedImg;
	ImageBitstream newImg;

	if(kernel == 0 || kernelSize % 2 == 0)
		return newImg;

	int imgrow;  // current row and col in the image where the filter is calculated
	int imgcol;
	int krow;  // current row and col in the kernel for the convolution sum
	int kcol;
	int row;  // row and col of image pixel for current kernel position
	int col;

	int offset = (kernelSize - 1) / 2;

	int extWidth = width_ + 2 * offset;
	int extHeight = height_ + 2 * offset;

	float sum;

	extendedImg = extendImage(offset);

	newImg.width_ = width_;
	newImg.height_ = height_;
	newImg.bitstream_ = new unsigned char[width_ * height_];

	for(imgrow = offset; imgrow < extHeight - offset; imgrow++)
	{
		for(imgcol = offset; imgcol < extWidth - offset; imgcol++)
		{
			sum = 0;

			// calculate weighted sum over kernel (convolution)
			for(krow = 0; krow < kernelSize; krow++)
			{
				for(kcol = 0; kcol < kernelSize; kcol++)
				{
					row = imgrow + krow - offset;
					col = imgcol + kcol - offset;

					sum += extendedImg[row * extWidth + col] * kernel[krow * kernelSize + kcol];
				}
			}

			newImg.bitstream_[(imgrow - offset) * width_ + (imgcol - offset)] = (unsigned char) round(sum);
		}
	}

	return newImg;
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



unsigned char *ImageBitstream::extendImage(int borderSize)
{
	int row;
	int col;
	unsigned char *extendedImage;

	int offset = borderSize;

	int extWidth = width_ + 2 * offset;
	int extHeight = height_ + 2 * offset;

	*extendedImg = new unsigned char[extWidth * extHeight];

	// step 0: copy image
	for(row = 0; row < height_; row++)
			for(col = 0; col < width_; col++)
					extendedImg[(row + offset) * extWidth + (col + offset)] = input_[row * width_ + col];

	// step 1a: copy upper border
	for(row = 0; row < offset; row++)
			for(col = 0; col < width_; col++)
					extendedImg[row * extWidth + (col + offset)] = input_[0 * width_ + col];

	// step 1b: copy lower border
	for(row = offset + height_; row < height_ + 2*offset; row++)
			for(col = 0; col < width_; col++)
					extendedImg[row * extWidth + (col + offset)] = input_[(height_ - 1) * width_ + col];

	// step 1c: copy left border
	for(col = 0; col < offset; col++)
			for(row = 0; row < height_; row++)
					extendedImg[(row + offset) * extWidth + col] = input_[row * width_ + 0];

	// step 1d: copy right border
	for(col = offset + width_; col < width_ + 2*offset; col++)
			for(row = 0; row < height_; row++)
					extendedImg[(row + offset) * extWidth + col] = input_[row * width_ + (width_ - 1)];

	// step 2a: copy upper left corner
	for(row = 0; row < offset; row++)
			for(col = 0; col < offset; col++)
					extendedImg[row * extWidth + col] = input_[0 * width_ + 0];

	// step 2b: copy upper right corner
	for(row = 0; row < offset; row++)
			for(col = offset + width_; col < width_ + 2*offset; col++)
					extendedImg[row * extWidth + col] = input_[0 * width_ + (width_ - 1)];

	// step 2c: copy lower left corner
	for(row = offset + height_; row < height_ + 2*offset; row++)
			for(col = 0; col < offset; col++)
					extendedImg[row * extWidth + col] = input_[(height_ - 1) * width_ + 0];

	// step 2d: copy lower right corner
	for(row = offset + height_; row < height_ + 2*offset; row++)
			for(col = offset + width_; col < width_ + 2*offset; col++)
					extendedImg[row * extWidth + col] = input_[(height_ - 1) * width_ + (width_ - 1)];

	return extendedImage;
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
	ImageBitstream newImg;
	unsigned char min = 255, max = 0;
	int pixel;

	// initialize new image bitstream
	newImg.width_ = width_;
	newImg.height_ = height_;
	newImg.bitstream_ = new unsigned char[width_ * height_];

	// get contrast range
	for(pixel = 0; pixel < width_ * height_; pixel++)
	{
		if(bitstream_[pixel] < min) min = bitstream_[pixel];
		if(bitstream_[pixel] > max) max = bitstream_[pixel];
	}

	// stretch contrast
	for(pixel = 0; pixel < width_ * height_; pixel++)
	{
		newImg.bitstream_[pixel] = (unsigned char) (((int) (bitstream_[pixel] - min)) * (int) 255) / ((int) max - min);
	}

	return newImg;
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