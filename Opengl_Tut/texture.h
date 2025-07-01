#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>

class texture
{
public:
	unsigned int id;
	unsigned int width, height;
	unsigned int internal_format;
	unsigned int image_format;
	unsigned int wrap_s, wrap_t;
	unsigned int filter_min, filter_mag;
	texture();
	void generate(unsigned int _width, unsigned int _height, unsigned char *data);
	void bind() const;
};

#endif

