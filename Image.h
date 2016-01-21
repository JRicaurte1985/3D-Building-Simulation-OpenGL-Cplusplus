#include <fstream>


using namespace std;

class Image
{
public:
	Image(char* ps, int w, int h);
	~Image();

	static int toInt(const char* bytes);
	static int readInt(ifstream &input);
	static Image* loadBMP(const char* filename);
	static GLuint loadTextureFromImage(Image* image);

private:
	char* pixels;
	int width;
	int height;
};


