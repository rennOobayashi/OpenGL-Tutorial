#include "texture.h"

texture::texture() : width(0), height(0), internal_format(GL_RGB), image_format(GL_REPEAT), 
	wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_min(GL_LINEAR), filter_mag(GL_LINEAR) {
	glGenTextures(1, &id);
} 

void texture::generate(unsigned int _width, unsigned int _height, unsigned char* data) {
	width = _width;
	height = _height;
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, id);
}