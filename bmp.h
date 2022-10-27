#ifndef __BMP_H
#define __BMP_H

#include <stdint.h>
#include <iostream>

// define IS_LITTLE_ENDIAN (!!(union { uint16_t u16; unsigned char c; }){ .u16 = 1 }.c)
#define IS_LITTLE_ENDIAN (*(uint16_t *)"\0\xff" > 0x100)

namespace JiMP2 {

/*
 * Informacje o formacie BMP https://en.wikipedia.org/wiki/BMP_file_format
 */

struct BMPFileHeader {
	char id[2];
	uint32_t size;
	char reserved[4];
	uint32_t dataOffset;

	BMPFileHeader() :
			id { 'B', 'M' }, size(0), dataOffset(0) {
		//id[0] = 'B';
		//id[1] = 'M';
	}
};

struct BitmapCoreHeader {
	 uint32_t size;
	 uint16_t bmpWidth;
	 uint16_t bmpHeight;
	 uint16_t colorPlanes;
	 uint16_t bitsPerPixel;

	BitmapCoreHeader(uint16_t width, uint16_t height) :
			size(12), bmpWidth(width), bmpHeight(height), colorPlanes(1),
			bitsPerPixel(24)
	{
	}

};

class BMP {
	BMPFileHeader bmpFileHeader;
	BitmapCoreHeader bitmapCoreHeader;
	unsigned char* pixelData;

public:

	BMP(uint16_t width, uint16_t height);
	virtual ~BMP();

	void setPixel(double x, double y, unsigned char r, unsigned char g,
			unsigned char b);
			
	void line(double x1, double y1, double x2, double y2, unsigned char r, unsigned char g,
		unsigned char b);
		
	void circle(uint16_t x, uint16_t y, uint16_t rad, unsigned char r, unsigned char g,
		unsigned char b);
		
	void circleFull(uint16_t x, uint16_t y, uint16_t rad, unsigned char r, unsigned char g,
		unsigned char b);
		
	void setPixelArc(uint16_t x0, uint16_t y0,uint16_t x, uint16_t y, uint16_t alpha, uint16_t beta, unsigned char r, unsigned char g,
		unsigned char b);
		
	void arc(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t alpha, uint16_t beta, unsigned char r, unsigned char g,
		unsigned char b);
	
	void lineArc(uint16_t x0, uint16_t y0, uint16_t x, uint16_t y, uint16_t alpha, uint16_t beta, unsigned char r, unsigned char g,
		unsigned char b);
	
	void arcFull(uint16_t x0, uint16_t y0, uint16_t alpha, uint16_t beta, uint16_t radius, unsigned char r, unsigned char g,
		unsigned char b);

	friend std::ostream& operator<<(std::ostream& os, const BMP& bmp);
};

}

#endif
