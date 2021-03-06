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

// screen 
enum Screen{
	QUIT_SCREEN,
	HOME_SCREEN,
	HELP_SCREEN,
	PAUSE_SCREEN,
	GAME_SCREEN
};

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
const int MOVE_DELAY = 800;

enum MoveType {
	MOVED,
	ROTATED,
	HOLD
};

enum Move{
	LEFT,
	RIGHT,
	DOWN,
	FORCE,
	DROP
};

enum Rotate{
	CW,
	CCW,
	FLIP
};

// piece

enum PiecesNumbering {
	NO_PIECE,
	I_PIECE,
	S_PIECE,
	J_PIECE,
	L_PIECE,
	T_PIECE,
	Z_PIECE,
	O_PIECE,
	GHOST_PIECE,
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
	GHOST_BLOCK,
	TOTAL_BLOCK_COLOR
};

// sfx
enum MenuSFX {
	MENU_HOVERED,
	MENU_CLICKED,
	TOTAL_MENU_SFX
};

// scoring 
const int NUMBER_WIDTH = 27;
const int NUMBER_HEIGHT = 36;

enum ScoringSystem {
	HARD_DROP,
	SINGLE,
	DOUBLE,
	TRIPLE,
	QUAD,
	T_SPIN,
	T_SPIN_SINGLE,
	T_SPIN_DOUBLE,
	T_SPIN_TRIPLE,
	PERFECT_CLEAR,
	TOTAL_SCORING_TYPE
};

enum Numbering {
	NUM_0,
	NUM_1,
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,
	NUM_x,
	TOTAL_NUMBER
};

void ErrorLog(std::string msg, int error_code);

#endif