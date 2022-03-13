#ifndef GAMEBASE_H
#define GAMEBASE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

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
const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 1000;

// board
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 23;

enum BoardStatus {
	POS_FREE,
	POS_FILLED
};

// piece

enum PiecesNumbering {
	O_PIECE,
	L_PIECE,
	REV_L_PIECE,
	I_PIECE,
	Z_PIECE,
	S_PIECE,
	T_PIECE,
	TOTAL_PIECE
};

enum BlockColor {
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