#include "Texture.hpp"

void Texture::SetupTexture(GLuint texture, string filePath, bool allowWrap){
	SDL_Surface * surface = IMG_Load(filePath.c_str());
	if (!surface) {
		printf("IMG_Load: %s\n", IMG_GetError());
		// handle error
		exit(1);
	}
	GLenum textureFormat;
	switch (surface->format->BytesPerPixel) {
	case 4:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			textureFormat = GL_BGRA;
		else
			textureFormat = GL_RGBA;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			textureFormat = GL_BGR;
		else
			textureFormat = GL_RGB;
		break;
	}

//	SDL_SaveBMP(surface, "test.bmp");

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, (GLint)GL_RGB8, surface->w,
		surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	// GL_RGB8 was previously surface->format->BytesPerPixel, but this doesn't work with core profile.
	// Maybe there should be logic here to decide what to use.

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)GL_LINEAR);

	if (!allowWrap){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)GL_CLAMP_TO_EDGE);
	}

	glTexParameterf(GL_TEXTURE_2D, (GLenum) GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(surface);

	cout << "Setup texture: " << filePath << endl;
}