#include "Texture.hpp"

void Texture::SetupTexture(GLuint texture, string filePath, bool allowWrap, bool filtering, uint8_t ** output, int * outHeight, int * outWidth){
	SDL_Surface * surface = IMG_Load(filePath.c_str());
	if (!surface) {
		printf("IMG_Load: %s\n", IMG_GetError());
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

	if (output != NULL){
		uint8_t * outputArray = new uint8_t[surface->w * surface->h * 4];
		(*outHeight) = surface->h;
		(*outWidth) = surface->w;
		(*output) = outputArray;
		for (int x = 0; x < surface->w; x++){
			for (int y = 0; y < surface->h; y++){
				int bpp = surface->format->BytesPerPixel;
				Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
				Uint32 pixelColor = *(Uint32*)p;
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a;
				SDL_GetRGBA(pixelColor, surface->format, &r, &g, &b, &a);
				outputArray[x * surface->w + y * 4 + 0] = r; 
				outputArray[x * surface->w + y * 4 + 1] = g;
				outputArray[x * surface->w + y * 4 + 2] = b;
				outputArray[x * surface->w + y * 4 + 3] = a;
			}
		}

	}

	//	SDL_SaveBMP(surface, "test.bmp");

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, (GLint)GL_RGB8, surface->w,
		surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	// GL_RGB8 was previously surface->format->BytesPerPixel, but this doesn't work with core profile.
	// Maybe there should be logic here to decide what to use.
	if (!allowWrap){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)GL_CLAMP_TO_EDGE);
	}
	if (filtering){
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, (GLenum)GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	}	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)GL_NEAREST);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(surface);
	cout << "Setup texture: " << filePath << endl;
}