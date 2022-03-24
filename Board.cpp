#include "Board.h"

bool Board::IsEnded() {
	for (int i = 0; i < BOARD_WIDTH; i++)
		if (board[i][0] != FREE_BLOCK)
			return true;
	return false;
}

void Board::DeleteRow(int row) {
	for (int i = row; i > 0; i--)
		for (int j = 0; j < BOARD_WIDTH; j++)
			board[i][j] = board[i][j - 1];
	for (int i = 0; i < BOARD_WIDTH; i++)
		board[0][i] = FREE_BLOCK;
}

void Board::DeletePosibleRow() {
	for (int j = 0; j < BOARD_HEIGHT; j ++){
		bool rowFilled = true;
		for (int i = 0; i < BOARD_WIDTH; i++)
			if (board[i][j] == FREE_BLOCK)
				rowFilled = false;
		if (rowFilled)
			DeleteRow(j);
	}
}

bool Board::IsPosibleMove(Piece p) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (p.shape[p.rotation][i][j] == '*') {
				int x = i + p.xOffSet, y = j + p.yOffSet;
				if (board[x][y] != FREE_BLOCK)
					return false;
			}
	return true;
}

void Board::DrawBoard(SDL_Renderer* renderer, Texture gBlock[]) {
	SDL_Rect gPieceClip = {0, 0, 22, 22};
	for (int i = 0; i < BOARD_HEIGHT; i++){
		for(int j = 0; j < BOARD_WIDTH; j++){
			if(board[i][j] != FREE_BLOCK){
				gBlock[board[i][j]].Render(renderer, 
				BOARD_START_X + BLOCK_HEIGHT * i,
				BOARD_START_Y + BLOCK_WIDTH * j, &gPieceClip);
			}
		}
	}
}

Board::Board() {
	for (int i = 0; i < BOARD_WIDTH; i++)
		for (int j = 0; j < BOARD_HEIGHT; j++)
			board[i][j] = FREE_BLOCK;
}

Board::~Board() {

}