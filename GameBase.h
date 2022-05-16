#ifndef GAMEBASE_H
#define GAMEBASE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <deque>

using namespace std;

// -- constant -- 

// error log
#define SDL_ERROR 1
#define IMG_ERROR 2
#define MIX_ERROR 3

// window
const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 640;

// board
const int BOARD_WIDTH = 20;
const int BOARD_HEIGHT = 15;

// snake
const int MOVE_TIME = 500;

enum Move{
	UP, 
	DOWN,
	LEFT,
	RIGHT
};

// button
enum ButtonSprite {
	BUTTON_DEFAULT,
	BUTTON_HOVERED,
	BUTTON_DOWN,
	BUTTON_TOTAL
};
void ErrorLog(std::string msg, int error_code);

#endif