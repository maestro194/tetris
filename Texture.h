#ifndef TEXTURE_H
#define TEXTURE_H

#include "GameBase.h"

class Texture {
public:
	Texture();
	~Texture();

	void FreeTexture();
	void LoadTextureFromFile(std::string path, SDL_Renderer* renderer);
	void Render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip);

	int GetWidth();
	int GetHeight();

private:
	SDL_Texture* texture;
	int width;
	int height;
};

#endif