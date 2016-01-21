#include <fstream>
#include "Image.h"

using namespace std;

Image::Image(char* ps, int w, int h) :pixels(ps), width(w), height(h) {}
Image::~Image() {}
int Image::toInt(const char* bytes)
{
	return (int)(((unsigned char)bytes[3] << 24) |
		((unsigned char)bytes[2] << 16) |
		((unsigned char)bytes[1] << 8) |
		(unsigned char)bytes[0]);
}
int Image::readInt(ifstream &input)
{
	char buffer[4];
	input.read(buffer, 4);
	return toInt(buffer);
}
Image* Image::loadBMP(const char* filename)
{
	try
	{
		ifstream infile(filename, ifstream::binary);
		infile.seekg(10, std::ios::cur);
		int dataOffset = readInt(infile);

		int headerSize = readInt(infile);
		int width = readInt(infile);
		int height = readInt(infile);

		int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
		int size = bytesPerRow * height;
		char* pixels = new char[size];
		infile.seekg(dataOffset, ios_base::beg);
		infile.read(pixels, size);
		infile.close();

		char* pixels2 = new char[width * height * 3];
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				for (int c = 0; c < 3; c++)
				{
					pixels2[3 * (width*y + x) + c] = pixels[bytesPerRow*y + 3 * x + (2 - c)];
				}
			}
		}
		delete[] pixels;
		return new Image(pixels2, width, height);
	}
	catch (int e)
	{
		return new Image(0, 0, 0);
	}
}
GLuint Image::loadTextureFromImage(Image* image)
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		image->width,
		image->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image->pixels);
	return textureId;
}
