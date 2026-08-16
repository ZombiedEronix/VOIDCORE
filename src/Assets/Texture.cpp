#include "Assets/Texture.h"
#include "stb_image.h"
Texture::Texture(const char* filename, GLint internalFormat)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(filename, &widthTexture, &heightTexture, &colorch, 0);
	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);


	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, widthTexture, heightTexture, 0, internalFormat, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}