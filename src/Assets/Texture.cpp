#include "Assets/Texture.h"
#include "stb_image.h"
#include "Core/Debug.h"
Texture::Texture(const char* filename, GLint internalFormat)
{
	stbi_set_flip_vertically_on_load(true);
	widthTexture = 0;
	heightTexture = 0;
	colorch = 0;
	unsigned char* pixels = stbi_load(filename, &widthTexture, &heightTexture, &colorch, 0);
	unsigned char whitePixel[4] = { 255, 255, 255, 255 };
	GLenum dataFormat = GL_RGBA;
	if (!pixels)
	{
		Debug::LogError("Texture not found, using 1x1 white: ", filename);
		widthTexture = 1;
		heightTexture = 1;
		pixels = whitePixel;
	}
	else
	{
		dataFormat = (colorch == 3) ? GL_RGB : GL_RGBA;
	}

	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, widthTexture, heightTexture, 0, dataFormat, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (pixels != whitePixel)
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