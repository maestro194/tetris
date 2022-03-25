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

using namespace std;

// -- constant -- 

// error log
#define SDL_ERROR 1
#define IMG_ERROR 2
#define MIX_ERROR 3

// button
enum ButtonSprite {
	BUTTON_DEFAULT,
	BUTTON_HOVERED,
	BUTTON_DOWN,
	BUTTON_TOTAL
};

// window
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// board
const int BOARD_WIDTH = 15;
const int BOARD_HEIGHT = 25;
const int BOARD_START_X = 484; // random value
const int BOARD_START_Y = 30; // random value
const int BLOCK_WIDTH = 26;
const int BLOCK_HEIGHT = 26;

// piece

enum PiecesNumbering {
	RANDOM_PIECE,
	I_PIECE,
	S_PIECE,
	J_PIECE,
	L_PIECE,
	T_PIECE,
	Z_PIECE,
	O_PIECE,
	TOTAL_PIECE
};

enum BlockColor {
	FREE_BLOCK,
	CYAN_BLOCK,
	LIME_BLOCK,
	NAVY_BLOCK,
	ORANGE_BLOCK,
	PINK_BLOCK,
	RED_BLOCK,
	YELLOW_BLOCK,
	TOTAL_BLOCK_COLOR
};


void ErrorLog(std::string msg, int error_code);

#endif