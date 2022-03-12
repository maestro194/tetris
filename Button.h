#ifndef BUTTON_H
#define BUTTON_H

#include "GameBase.h"
#include "Texture.h"

class Button {
public:
	Button();
	Button(int x, int y, int w, int h);
	~Button();

	int buttonSprite;
	
	bool CursorOnButton(int x, int y);
	void Render(SDL_Renderer* renderer, Texture ButtonTexture, SDL_Rect* clip);
	void HandleEvent(SDL_Event* e);
private:
	SDL_Point buttonPosition;
	int buttonWidth,
			buttonHeight;
};

#endif