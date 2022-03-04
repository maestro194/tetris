#include "Board.h"

bool Board::IsEnded() {
	for (int i = 0; i < BOARD_WIDTH; i++)
		if (CurrentBoard[i][0] == POS_FILLED)
			return true;
	return false;
}

Board::DeleteRow(int row) {
	for (int j = row; j > 0; j--)
		for (int i = 0; i < BOARD_WIDTH; i++)
			CurrentBoard[i][j] = CurrentBoard[i][j - 1];
	for (int i = 0; i < BOARD_WIDTH; i++)
		CurrentBoard[i][0] = POS_FREE;
}

Board::DeletePosibleRow() {
	for (int j = 0; j < BOARD_HEIGHT; j ++){
		bool rowFilled = true;
		for (int i = 0; i < BOARD_WIDTH; i++)
			if (CurrentBoard[i][j] == POS_FREE)
				rowFilled = false;
		if (rowFilled)
			DeleteLine(j);
	}
}

bool Board::IsPosibleMove() {

}

Board::DrawBoard(SDL_Renderer* renderer) {

}

Board::Board() {
	for (int i = 0; i < BOARD_WIDTH; i++)
		for (int j = 0; j < BOARD_HEIGHT; j++)
			CurrentBoard[i][j] = POS_FREE;
}

Board::~Board() {

}