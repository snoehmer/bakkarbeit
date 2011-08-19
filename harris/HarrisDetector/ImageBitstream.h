/*
 * ImageBitstream.h
 *
 *  Created on: 19.08.2011
 *      Author: sn
 */

#ifndef IMAGEBITSTREAM_H_
#define IMAGEBITSTREAM_H_

#include <cstring>
class Magick::Image;

/**
 * @class ImageBitstream
 * is an image converted to grayscale and represented as uchar array
 * 1 pixel equals 1 array element
 */
class ImageBitstream
{
public:

	ImageBitstream();
	ImageBitstream(const ImageBitstream &original);  // copy constructor
	ImageBitstream(Magick::Image img);
	ImageBitstream(std::string filename);
	virtual ~ImageBitstream();

	void setImage(Magick::Image img);
	void setImage(std::string filename);
	Magick::Image getImage();
	void saveImage(std::string filename);

	int getWidth();
	int getHeight();
	unsigned char* getBitstream();
	unsigned char* copyBitstream();
	unsigned char& pixel(int row, int col);

	ImageBitstream convolve(float *kernel, int kernelSize);
	ImageBitstream stretchContrast();

	ImageBitstream& operator=(ImageBitstream& original);

private:

	unsigned char *bitstream_;
	int width_;
	int height_;

	unsigned char* extendImage();
};

#endif /* IMAGEBITSTREAM_H_ */
