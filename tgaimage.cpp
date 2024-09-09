#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string.h>
#include <time.h>
#include <math.h>
#include "tgaimage.h"

template<class T, class V> 
void ret(T& t, V& v){
    return t + v;
}

typedef unsigned long ulong;
typedef unsigned char uchar;

TGAImage::TGAImage(): data(nullptr), width(100), height(100) {}
TGAImage::TGAImage(int w, int h, int bpp): width(w), height(h), bytespp(bpp) {
    ulong bytes = width * height * bytespp;
    this->data = new uchar[bytes];
    memset(this->data, 0, sizeof(this->data));
}
TGAImage::TGAImage(const TGAImage &img) {
	width = img.width;
	height = img.height;
	bytespp = img.bytespp;
	ulong nbytes = width*height*bytespp;
	data = new uchar[nbytes];
	memcpy(data, img.data, nbytes);
}

TGAImage::~TGAImage() {
	if (data) delete [] data;
}

TGAImage& TGAImage::operator=(const TGAImage &img) {
	if (this != &img) {
		if (data) delete [] data;
		width  = img.width;
		height = img.height;
		bytespp = img.bytespp;
		unsigned long nbytes = width*height*bytespp;
		data = new unsigned char[nbytes];
		memcpy(data, img.data, nbytes);
	}
	return *this;
}

bool TGAImage::read_tga_file(const char* filename) {
    if (this->data != nullptr) { delete [] data; }

    std::ifstream fin;
    fin.open(filename, std::ios::binary); 
    if (!fin.is_open() ){
        std::cerr << "Not able to read from file!" << std::endl;
    }
    TGA_Header hdr; // verify the header of the file is that of a TGA file.
    fin.read( (char*)&hdr, sizeof(hdr)); //read the first sizeof(hdr) bytes from the filename object on disk (this will get the header)
    if (!fin.good() ){
        std::cerr << "Stream is not in a condition for reading from disk!" << std::endl;
    }
    width = hdr.width;
	height = hdr.height;
	bytespp = hdr.bitsperpixel>>3;
	if (width<=0 || height<=0 || (bytespp!=GRAYSCALE && bytespp!=RGB && bytespp!=RGBA)) {
		fin.close();
		std::cerr << "bad bpp (or width/height) value\n";
		return false;
	}
	unsigned long nbytes = bytespp*width*height;
	data = new unsigned char[nbytes];
	if (3==hdr.datatypecode || 2==hdr.datatypecode) {
		fin.read((char *)data, nbytes);
		if (!in.good()) {
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	} else if (10==hdr.datatypecode||11==hdr.datatypecode) {
		if (!load_rle_data(fin)) {
			fin.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	} else {
		fin.close();
		std::cerr << "unknown file format " << (int)hdr.datatypecode << "\n";
		return false;
	}
	if (!(hdr.imagedescriptor & 0x20)) {
		flip_vertically();
	}
	if (hdr.imagedescriptor & 0x10) {
		flip_horizontally();
	}
	std::cerr << width << "x" << height << "/" << bytespp*8 << "\n";
	fin.close();
	return true;
}

bool TGAImage::load_rle_data(std::ifstream& in){

}
bool TGAImage::write_tga_file(const char* filename, bool rle=true) {

}
bool TGAImage::unload_rle_data(std::ofstream& out){
    
}

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
    for (float f = 0; f < 1; f += 0.01) {
        float xp = x0 + (x1 - x0) * f;
        float yp = y0 + (y1 - y0) * f;
        image.set(xp, yp, color);
    }
}

int main(void){

}