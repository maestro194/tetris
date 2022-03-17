#include "Board.h"
#include "GameBase.h"

bool Board::IsEnded() {
	for (int i = 0; i < BOARD_WIDTH; i++)
		if (CurrentBoard[i][0] == POS_FILLED)
			return true;
	return false;
}

void Board::DeleteRow(int row) {
	for (int i = row; i > 0; i--)
		for (int j = 0; j < BOARD_WIDTH; j++)
			CurrentBoard[i][j] = CurrentBoard[i][j - 1];
	for (int i = 0; i < BOARD_WIDTH; i++)
		CurrentBoard[0][i] = POS_FREE;
}

void Board::DeletePosibleRow() {
	for (int j = 0; j < BOARD_HEIGHT; j ++){
		bool rowFilled = true;
		for (int i = 0; i < BOARD_WIDTH; i++)
			if (CurrentBoard[i][j] == POS_FREE)
				rowFilled = false;
		if (rowFilled)
			DeleteRow(j);
	}
}

bool Board::IsPosibleMove() {

}

void Board::DrawBoard(SDL_Renderer* renderer) {
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{

	}
}

Board::Board() {
	for (int i = 0; i < BOARD_WIDTH; i++)
		for (int j = 0; j < BOARD_HEIGHT; j++)
			CurrentBoard[i][j] = POS_FREE;
}

Board::~Board() {

}