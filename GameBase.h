#ifndef GAMEBASE_H
#define GAMEBASE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

// -- constant -- 

// error log
#define SDL_ERROR 1
#define IMG_ERROR 2

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
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 23;
const int BOARD_START_X = 30; // random value
const int BOARD_START_Y = 30; // random value
const int BLOCK_WIDTH = 22;
const int BLOCK_HEIGHT = 22;
const int	PIECE_START_BLOCK_X = 1;
const int PIECE_START_BLOCK_Y = 4;

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