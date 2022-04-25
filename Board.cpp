#include "Board.h"

void Board::Reset() {
	for (int i = 0; i < BOARD_HEIGHT; i++)
		for (int j = 0; j < BOARD_WIDTH; j++)
			board[i][j] = CYAN_BLOCK;
	for(int i = 0; i < 23; i ++)
		for(int j = 3; j < 13; j ++)
			board[i][j] = FREE_BLOCK;
}

bool Board::IsEnded() {
	for (int i = 3; i < 13; i++)
		if (board[0][i] != FREE_BLOCK)
			return true;
	return false;
}

void Board::DeleteRow(int row) {
	for (int i = row; i > 0; i--)
		for (int j = 3; j < 13; j++)
			board[i][j] = board[i - 1][j];
	for (int i = 3; i < 13; i++)
		board[0][i] = FREE_BLOCK;
}

int Board::DeletePosibleRow() {
	int cnt = 0;
	for (int i = 1; i < 23; i ++){
		bool rowFilled = true;
		for (int j = 3; j < 13; j ++)
			if (board[i][j] == FREE_BLOCK)
				rowFilled = false;
		if (rowFilled)
			DeleteRow(i),
			cnt ++;
	}
	
	return cnt;
}

bool Board::IsPosibleMove(Piece p) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (p.shape[p.rotation][i][j] == 1) {
				int x = i + p.xOffSet, y = j + p.yOffSet;
				if (board[x][y] != FREE_BLOCK)
					return false;
			}
	return true;
}

void Board::MergePiece(Piece p) {
	for(int i = 0; i < 4; i ++)
		for(int j = 0; j < 4; j ++)
			if(p.shape[p.rotation][i][j] == 1){
				int x = i + p.xOffSet, y = j + p.yOffSet;
				board[x][y] = p.pieceType;
			}
}

bool Board::TSpinDetection(Piece p) {
	int x = p.xOffSet + 1, y = p.yOffSet + 1;
	int cnt = 0;

	if(y == 22)
		return false;
	
	if(board[x - 1][y - 1] != FREE_BLOCK) cnt ++;
	if(board[x - 1][y + 1] != FREE_BLOCK) cnt ++;
	if(board[x + 1][y - 1] != FREE_BLOCK) cnt ++;
	if(board[x + 1][y + 1] != FREE_BLOCK) cnt ++;

	return (cnt >= 3);
}

int Board::LineClearType(Piece p){
	bool TSpinFlag = (p.pieceType == T_PIECE && TSpinDetection(p));
	MergePiece(p);
	int lineClear = DeletePosibleRow();
	if(TSpinFlag) 
		return T_SPIN + lineClear;
	
	else {
		bool pc = 1;
		for(int j = 3; j < 13; j ++)
			if(board[22][j] != FREE_BLOCK)
				pc = 0;
			
		if(pc)
			return ALL_CLEAR;
		else
			return DROP + lineClear;
	}
}

void Board::DrawBoard(SDL_Renderer* renderer, Texture gBlock[]) {
	SDL_Rect gPieceClip = {0, 0, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1};
	for (int i = 1; i < 23; i ++) {
		for(int j = 3; j < 13; j ++) {
			if(board[i][j] != FREE_BLOCK) {
				gBlock[board[i][j]].Render(renderer, 
				BOARD_START_X + BLOCK_WIDTH * (j - 3), 
				BOARD_START_Y + BLOCK_HEIGHT * (i - 1), &gPieceClip);
			}
		}
	}
}

Board::Board() {
	for (int i = 0; i < BOARD_HEIGHT; i++)
		for (int j = 0; j < BOARD_WIDTH; j++)
			board[i][j] = CYAN_BLOCK;
	for(int i = 0; i < 23; i ++)
		for(int j = 3; j < 13; j ++)
			board[i][j] = FREE_BLOCK;
}

Board::~Board() {

}