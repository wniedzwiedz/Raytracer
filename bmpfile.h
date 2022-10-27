#include "bmp.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <cstring>
#include <cmath>


using namespace std;

namespace JiMP2 {

BMP::BMP(uint16_t width, uint16_t height) :
		bitmapCoreHeader(width, height) {

	assert(IS_LITTLE_ENDIAN);
	assert(width > 0);
	assert(height > 0);

	const unsigned int rowSize = ((bitmapCoreHeader.bitsPerPixel * width + 31)
			/ 32) * 4;
	const unsigned int imgSize = rowSize * height;

	bmpFileHeader.size = 14 + bitmapCoreHeader.size + imgSize;
	bmpFileHeader.dataOffset = 14 + bitmapCoreHeader.size;
//std::cout<<imgSize<<std::endl;
	pixelData = new unsigned char[imgSize];
	std::memset(pixelData, 255, imgSize);
}

BMP::~BMP() {
	delete[] pixelData;
}

void BMP::setPixel(double x, double y, unsigned char r, unsigned char g,
		unsigned char b) {
	assert(bitmapCoreHeader.bitsPerPixel == 24);

	const size_t rowSize = ((bitmapCoreHeader.bitsPerPixel
			* bitmapCoreHeader.bmpWidth + 31) / 32) * 4;
			//std::cout<<rowSize<<" ";
	const size_t offset = rowSize * (bitmapCoreHeader.bmpHeight - y -1)
			+ x * (bitmapCoreHeader.bitsPerPixel / 8);
			//std::cout<<offset<<" bitmapCoreHeader.bmpHeight: "<<bitmapCoreHeader.bmpHeight<<" y: "<<y<<" wynik: "<<(bitmapCoreHeader.bmpHeight - y)<<std::endl;

	pixelData[offset + 0] = b;
	pixelData[offset + 1] = g;
	pixelData[offset + 2] = r;
}

std::ostream& operator<<(std::ostream& os, const BMP& bmp) {
	os.write(bmp.bmpFileHeader.id, sizeof(bmp.bmpFileHeader.id));
	os.write((const char*) &bmp.bmpFileHeader.size,
			sizeof(bmp.bmpFileHeader.size));
	os.write(bmp.bmpFileHeader.reserved, sizeof(bmp.bmpFileHeader.reserved));
	os.write((const char*) &bmp.bmpFileHeader.dataOffset,
			sizeof(bmp.bmpFileHeader.dataOffset));

	os.write((const char*) &bmp.bitmapCoreHeader.size,
			sizeof(bmp.bitmapCoreHeader.size));
	os.write((const char*) &bmp.bitmapCoreHeader.bmpWidth,
			sizeof(bmp.bitmapCoreHeader.bmpWidth));
	os.write((const char*) &bmp.bitmapCoreHeader.bmpHeight,
			sizeof(bmp.bitmapCoreHeader.bmpHeight));
	os.write((const char*) &bmp.bitmapCoreHeader.colorPlanes,
			sizeof(bmp.bitmapCoreHeader.colorPlanes));
	os.write((const char*) &bmp.bitmapCoreHeader.bitsPerPixel,
			sizeof(bmp.bitmapCoreHeader.bitsPerPixel));

	const unsigned int rowSize = ((bmp.bitmapCoreHeader.bitsPerPixel
			* bmp.bitmapCoreHeader.bmpWidth + 31) / 32) * 4;
	const unsigned int imgSize = rowSize * bmp.bitmapCoreHeader.bmpHeight;

	os.write((const char*) bmp.pixelData, imgSize);

	return os;
}

void BMP::line(double x0, double y0, double x1, double y1, unsigned char r, unsigned char g,
		unsigned char b) {
  double side = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  double up = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  double err = (side>up ? side : -up)/2, e2;
 
  for(;;){
    setPixel(x0,y0,r,g,b);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-side) { err -= up; x0 += sx; }
    if (e2 < up) { err += side; y0 += sy; }
  }
	}
	
	void BMP::circle(uint16_t x0, uint16_t y0, uint16_t radius, unsigned char r, unsigned char g,
		unsigned char b) {
	int x = radius-1;
    int y = 0;
    int side = 1;
    int up = 1;
    int err = side - (radius << 1);

    while (x >= y)
    {
        setPixel(x0 + x, y0 + y, r,g,b);
        setPixel(x0 + y, y0 + x, r,g,b);
        setPixel(x0 - y, y0 + x, r,g,b);
        setPixel(x0 - x, y0 + y, r,g,b);
        setPixel(x0 - x, y0 - y, r,g,b);
        setPixel(x0 - y, y0 - x, r,g,b);
        setPixel(x0 + y, y0 - x, r,g,b);
        setPixel(x0 + x, y0 - y, r,g,b);

        if (err <= 0)
        {
            y++;
            err += up;
            up += 2;
        }
        
        if (err > 0)
        {
            x--;
            side += 2;
            err += side - (radius << 1);
        }
    }
	}
	
	void BMP::circleFull(uint16_t x0, uint16_t y0, uint16_t radius, unsigned char r, unsigned char g,
		unsigned char b) {
	int x = radius-1;
    int y = 0;
    int side = 1;
    int up = 1;
    int err = side - (radius << 1);

    while (x >= y)
    {
        line(x0 + x, y0 + y, x0 - x, y0 + y, r,g,b);
        line(x0 + y, y0 + x, x0 - y, y0 + x, r,g,b);
        line(x0 - x, y0 - y, x0 + x, y0 - y, r,g,b);
        line(x0 - y, y0 - x, x0 + y, y0 - x, r,g,b);

        if (err <= 0)
        {
            y++;
            err += up;
            up += 2;
        }
        
        if (err > 0)
        {
            x--;
            side += 2;
            err += side - (radius << 1);
        }
    }
	}
	
	void BMP::setPixelArc(uint16_t x0, uint16_t y0, uint16_t x, uint16_t y, uint16_t alpha, uint16_t beta, unsigned char r, unsigned char g,
		unsigned char b) {
	double tan = atan2(y-y0,x-x0);
	if(alpha<=tan&&tan<=beta){
		setPixel(x,y,r,g,b);
			}
}
	void BMP::arc(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t alpha, uint16_t beta, unsigned char r, unsigned char g,
		unsigned char b) {   
	
	int x = radius-1;
    int y = 0;	 
	int side = 1;
    int up = 1;
    int err = side - (radius << 1);	
    while (x >= y)

    {
    	setPixelArc(x0,y0,x0 + x, y0 + y,alpha,beta, r,g,b);
    	setPixelArc(x0,y0,x0 + y, y0 + x,alpha,beta, r,g,b);
        setPixelArc(x0,y0,x0 - y, y0 + x,alpha,beta, r,g,b);
        setPixelArc(x0,y0,x0 - x, y0 + y,alpha,beta, r,g,b);
        setPixelArc(x0,y0,x0 - x, y0 - y,alpha,beta, r,g,b);
        setPixelArc(x0,y0,x0 - y, y0 - x,alpha,beta, r,g,b);
        setPixelArc(x0,y0,x0 + y, y0 - x,alpha,beta, r,g,b);
        setPixelArc(x0,y0,x0 + x, y0 - y,alpha,beta, r,g,b);

        
    }
        if (err <= 0)
        {
            y++;
            err += up;
            up += 2;
        }
        
        if (err > 0)
        {
            x--;
            side += 2;
            err += side - (radius << 1);
        }
    }
		
	void BMP::lineArc(uint16_t x0, uint16_t y0, uint16_t x, uint16_t y, uint16_t alpha, uint16_t beta, unsigned char r, unsigned char g,
		unsigned char b) {
	double tan = atan2(y-y0,x-x0);
	if(alpha<=tan&&tan<=beta){
	
		line(x0,y0,x,y,r,g,b);
		
			}
}

	void BMP::arcFull(uint16_t x0, uint16_t y0, uint16_t alpha, uint16_t beta, uint16_t radius, unsigned char r, unsigned char g,
		unsigned char b) {
	int x = radius-1;
    int y = 0;
    int side = 1;
    int up = 1;
    int err = side - (radius << 1);

    while (x >= y)
    {
        lineArc(x0,y0,x0 + x, y0 + y,alpha,beta, r,g,b);
        lineArc(x0,y0,x0 + y, y0 + x,alpha,beta, r,g,b);
        lineArc(x0,y0,x0 - y, y0 + x,alpha,beta, r,g,b);
        lineArc(x0,y0,x0 - x, y0 + y,alpha,beta, r,g,b);
        lineArc(x0,y0,x0 - x, y0 - y,alpha,beta, r,g,b);
        lineArc(x0,y0,x0 - y, y0 - x,alpha,beta, r,g,b);
        lineArc(x0,y0,x0 + y, y0 - x,alpha,beta, r,g,b);
        lineArc(x0,y0,x0 + x, y0 - y,alpha,beta, r,g,b);

        if (err <= 0)
        {
            y++;
            err += up;
            up += 2;
        }
        
        if (err > 0)
        {
            x--;
            side += 2;
            err += side - (radius << 1);
        }
    }
}
		
	

}


