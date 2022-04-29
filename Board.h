#ifndef BOARD_H
#define BOARD_H

#include "GameBase.h"
#include "Piece.h"
#include "Texture.h"

class Board {
public:
	Board();
	~Board();

	bool IsEnded();

	void Reset();

	void InitWallKick();

	void Move(int moveType);
	void Rotate(int moveType);
	void Hold();
	bool ForceMove();

	void NextPiece();
	void FirstPieceBag();
	void SecondPieceBag();

	void DeleteRow(int row);
	int DeletePosibleRow();
	bool IsPosibleMove();
	void MergePiece();
	bool TSpinDetection();
	int LineClearType();

	void DrawBoard(SDL_Renderer* renderer);
	void TextureInit(SDL_Renderer* renderer);

private:
	int board[BOARD_HEIGHT][BOARD_WIDTH];
	Texture boardTex;
	Texture playFieldBG;
	Texture block[TOTAL_BLOCK_COLOR];

	SDL_Rect boardTexClip = {0, 0, 1280, 720};
	SDL_Rect playFieldBGClip = { 0, 0, 1280, 720 };
	SDL_Rect blockClip = {0, 0, BLOCK_WIDTH, BLOCK_HEIGHT};

	Piece currentPiece;
	Piece holdPiece;
	Piece nextPiece[3];
	int holdPieceType;
	int incomingPiece[15];
	int pieceCount;
	int holdThisTurn;

	int wallKickData[2][4][2][5][2];
};

#endif