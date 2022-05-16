#include "Texture.h"

Texture::Texture() {
	texture = NULL;
	width = 0;
	height = 0;
}

Texture::~Texture() {
	FreeTexture();
}

void Texture::FreeTexture() {
	if (texture != NULL) {
		texture = NULL;
		width = 0;
		height = 0;
	}
}

void Texture::LoadTextureFromFile(std::string path, SDL_Renderer* renderer) {
	SDL_Texture* tmpTexture = NULL;
	SDL_Surface* tmpSurface = IMG_Load(path.c_str());

	if (tmpSurface == NULL)
		ErrorLog("Cant load image. ", IMG_ERROR);
	else {
		// SDL_SetColorKey();

		tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
		if (tmpTexture == NULL)
			ErrorLog("Cant create texture. ", SDL_ERROR);
		else {
			width = tmpSurface->w;
			height = tmpSurface->h;
		}
		
		SDL_FreeSurface(tmpSurface);
	}

	texture = tmpTexture;
}

void Texture::Render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip) {
	SDL_Rect renderSpace = { x, y, width, height };

	if (clip != NULL) {
		renderSpace.w = clip->w;
		renderSpace.h = clip->h;
	}

	SDL_RenderCopy(renderer, texture, NULL, &renderSpace);
}

int Texture::GetWidth() {
	return width;
}

int Texture::GetHeight() {
	return height;
}