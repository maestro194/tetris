#ifndef GAMEBASE_H
#define GAMEBASE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

// -- constant -- 

// button

// window
const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 1000;

// board
const int BOARD_LINE_WIDTH = 6;
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;

enum BoardStatus {
	POS_FREE,
	POS_FILLED
};

// piece

enum PiecesNumbering {
	SQUARE_PIECE,
	L_PIECE,
	REV_L_PIECE,
	LINE_PIECE,
	Z_PIECE,
	REV_Z_PIECE,
	T_PIECE,
	TOTAL_PIECE
};
/*
enum PiecesColor {
	YELLOW_PIECE,
	ORANGE_PIECE,
	NAVY_PIECE,
	CYAN_PIECE,
	RED_PIECE,
	LIME_PIECE,
	PINK_PIECE,
	TOTAL_PIECE_COLOR
};
*/

void ErrorLog(std::string msg, int error_code);

#endif