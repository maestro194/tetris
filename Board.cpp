#include "Board.h"

void Board::Reset() {
	for (int i = 0; i < BOARD_HEIGHT; i++)
		for (int j = 0; j < BOARD_WIDTH; j++)
			board[i][j] = CYAN_BLOCK;
	for(int i = 0; i < 23; i ++)
		for(int j = 3; j < 13; j ++)
			board[i][j] = FREE_BLOCK;

	pieceCount = 0;
  holdPieceType = NO_PIECE;
	FirstPieceBag();
	SecondPieceBag();
  currentPiece.Init(incomingPiece[pieceCount]);
	currentPieceGhost.Init(incomingPiece[pieceCount]);
	UpdateGhostPiece();
	for(int i = 0; i < 3; i ++){
		nextPiece[i].Init(incomingPiece[pieceCount + 1 + i]);
		nextPiece[i].xOffSet = 5 + 4 * i;
		nextPiece[i].yOffSet = 14;
	}
	holdThisTurn = 0;
}

bool Board::IsEnded() {
	for (int i = 3; i < 13; i++)
		if (board[0][i] != FREE_BLOCK)
			return true;
	return false;
}

void Board::Move(int moveType){
	if(moveType == LEFT){
		currentPiece.PieceLeftMove();
		if(!IsPosibleMove())
			currentPiece.PieceRightMove();
	}
	else if(moveType == RIGHT){
		currentPiece.PieceRightMove();
		if(!IsPosibleMove())
			currentPiece.PieceLeftMove();
	}
	else if(moveType == DOWN){
		currentPiece.PieceDownMove();
		if(!IsPosibleMove())
			currentPiece.PieceUpMove();
	}
	else{
		do{
			currentPiece.PieceDownMove();
		}while(IsPosibleMove());
		currentPiece.PieceUpMove();
		holdThisTurn = 0;
	}
	
	UpdateGhostPiece();
}

void Board::Rotate(int moveType){
	int xKick, yKick;
	if(moveType == CCW){
		if(currentPiece.pieceType == O_PIECE){
			return;
		}
		else if(currentPiece.pieceType == I_PIECE){
			for(int i = 0; i < 5; i ++) {
				xKick = wallKickData[1][currentPiece.rotation][1][i][0];
				yKick = wallKickData[1][currentPiece.rotation][1][i][1];
				currentPiece.PieceCCWRotateMove(xKick, yKick);
				if (!IsPosibleMove())
					currentPiece.PieceCWRotateMove(-xKick, -yKick);
				else
					break;
			}
		}
		else{
			for(int i = 0; i < 5; i ++) {
				xKick = wallKickData[0][currentPiece.rotation][1][i][0];
				yKick = wallKickData[0][currentPiece.rotation][1][i][1];   
				currentPiece.PieceCCWRotateMove(xKick, yKick);
				if(!IsPosibleMove())
					currentPiece.PieceCWRotateMove(-xKick, -yKick);
				else  
					break;
			}
		}
	}
	else if(moveType == CW){
		if(currentPiece.pieceType == O_PIECE){
			return;
		}
		else if(currentPiece.pieceType == I_PIECE){
			for(int i = 0; i < 5; i ++) {
				xKick = wallKickData[1][currentPiece.rotation][0][i][0];
				yKick = wallKickData[1][currentPiece.rotation][0][i][1];
				currentPiece.PieceCWRotateMove(xKick, yKick);
				if (!IsPosibleMove())
					currentPiece.PieceCCWRotateMove(-xKick, -yKick);
				else
					break;
			}
		}
		else{
			for(int i = 0; i < 5; i ++) {
				xKick = wallKickData[0][currentPiece.rotation][0][i][0];
				yKick = wallKickData[0][currentPiece.rotation][0][i][1];   
				currentPiece.PieceCWRotateMove(xKick, yKick);
				if(!IsPosibleMove())
					currentPiece.PieceCCWRotateMove(-xKick, -yKick);
				else  
					break;
			}
		}
	}
	else{
		currentPiece.PieceFlipMove();
		if (!IsPosibleMove())
			currentPiece.PieceFlipMove();
	}
	UpdateLastMove(ROTATED);
	UpdateGhostPiece();
}

void Board::Hold(){
	if(holdThisTurn == 1)
		return;

	holdThisTurn = 1;

	if(holdPieceType == NO_PIECE) {
		holdPieceType = currentPiece.pieceType;
		holdPiece.Init(holdPieceType);

		pieceCount = (pieceCount + 1) % 14;
		if (pieceCount == 0)
			SecondPieceBag();
		if (pieceCount == 7)
			FirstPieceBag();
		currentPiece.Init(incomingPiece[pieceCount]);
		currentPieceGhost.Init(incomingPiece[pieceCount]);
		for(int i = 0; i < 3; i ++){
			nextPiece[i].Init(incomingPiece[(pieceCount + 1 + i) % 14]);
			nextPiece[i].xOffSet = 5 + 4 * i;
			nextPiece[i].yOffSet = 14;
		}
	}
	else {
		swap(incomingPiece[pieceCount], holdPieceType);
		holdPiece.Init(holdPieceType);

		currentPiece.Init(incomingPiece[pieceCount]);
		currentPieceGhost.Init(incomingPiece[pieceCount]);
	}
	holdPiece.xOffSet = 5;
	holdPiece.yOffSet = -2;
	UpdateLastMove(HOLD);
	UpdateGhostPiece();
}

bool Board::ForceMove(){
	currentPiece.PieceDownMove();
	if(!IsPosibleMove()){
		currentPiece.PieceUpMove();
		holdThisTurn = 0;
		UpdateGhostPiece();
		return true;
	}
	else
		UpdateLastMove(MOVED);
	UpdateGhostPiece();
	return false;
}

void Board::NextPiece(){
	pieceCount = (pieceCount + 1) % 14;
	if (pieceCount == 0)
		SecondPieceBag();
	if (pieceCount == 7)
		FirstPieceBag();
	currentPiece.Init(incomingPiece[pieceCount]);
	currentPieceGhost.Init(incomingPiece[pieceCount]);
	for(int i = 0; i < 3; i ++){
		nextPiece[i].Init(incomingPiece[(pieceCount + 1 + i) % 14]);
		nextPiece[i].xOffSet = 5 + 4 * i;
		nextPiece[i].yOffSet = 14;
	}
	UpdateGhostPiece();
}

void Board::FirstPieceBag() {
	int arrayForRandom[7] = {1,2,3,4,5,6,7};
  random_shuffle(arrayForRandom, arrayForRandom + 7);
  for (int i = 0; i < 7; i++)
    incomingPiece[i] = arrayForRandom[i];
}

void Board::SecondPieceBag() {
	int arrayForRandom[7] = {1,2,3,4,5,6,7};
  random_shuffle(arrayForRandom, arrayForRandom + 7);
  for (int i = 7; i < 14; i++)
    incomingPiece[i] = arrayForRandom[i - 7];
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

bool Board::IsPosibleMove() {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (currentPiece.shape[currentPiece.rotation][i][j] == 1) {
				int x = i + currentPiece.xOffSet, y = j + currentPiece.yOffSet;
				if (board[x][y] != FREE_BLOCK)
					return false;
			}
	return true;
}

bool Board::IsPosibleMoveGhost() {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (currentPieceGhost.shape[currentPieceGhost.rotation][i][j] == 1) {
				int x = i + currentPieceGhost.xOffSet, y = j + currentPieceGhost.yOffSet;
				if (board[x][y] != FREE_BLOCK)
					return false;
			}
	return true;
}

bool Board::GroundMoveDelay() {
	return currentPiece.xOffSet == currentPieceGhost.xOffSet;
}

void Board::MergePiece() {
	for(int i = 0; i < 4; i ++)
		for(int j = 0; j < 4; j ++)
			if(currentPiece.shape[currentPiece.rotation][i][j] == 1){
				int x = i + currentPiece.xOffSet, y = j + currentPiece.yOffSet;
				board[x][y] = currentPiece.pieceType;
			}
}

void Board::UpdateLastMove(int move) {
	lastMoveType = move;
}

int Board::GetLastMove() {
	return lastMoveType;
}

bool Board::TSpinDetection() {
	int x = currentPiece.xOffSet + 1, y = currentPiece.yOffSet + 1;
	int cnt = 0;

	if(y == 22)
		return false;
	if (lastMoveType != ROTATED)
		return false;
	
	if(board[x - 1][y - 1] != FREE_BLOCK) cnt ++;
	if(board[x - 1][y + 1] != FREE_BLOCK) cnt ++;
	if(board[x + 1][y - 1] != FREE_BLOCK) cnt ++;
	if(board[x + 1][y + 1] != FREE_BLOCK) cnt ++;

	return (cnt >= 3);
}

int Board::LineClearType(){
	bool TSpinFlag = (currentPiece.pieceType == T_PIECE && TSpinDetection());
	MergePiece();
	int lineClear = DeletePosibleRow();
	if(TSpinFlag) 
		return T_SPIN + lineClear;
	
	else {
		bool pc = 1;
		for(int j = 3; j < 13; j ++)
			if(board[22][j] != FREE_BLOCK)
				pc = 0;
			
		if(pc)
			return PERFECT_CLEAR;
		else
			return HARD_DROP + lineClear;
	}
}

void Board::UpdateGhostPiece() {
	currentPieceGhost.xOffSet = currentPiece.xOffSet;
	currentPieceGhost.yOffSet = currentPiece.yOffSet;
	currentPieceGhost.rotation = currentPiece.rotation;
	do {
		currentPieceGhost.PieceDownMove();
	} while (IsPosibleMoveGhost());
	currentPieceGhost.PieceUpMove();
}

void Board::DrawGhostPiece(SDL_Renderer* renderer) {
	currentPieceGhost.pieceType = currentPiece.pieceType;
	currentPieceGhost.DrawPiece(renderer, block);
	currentPieceGhost.pieceType = GHOST_PIECE;
	currentPieceGhost.DrawPiece(renderer, block);
}

void Board::DrawBoard(SDL_Renderer* renderer) {
	playFieldBG.Render(renderer, 0, 0, &playFieldBGClip);
	boardTex.Render(renderer, 0, 0, &boardTexClip);

	// draw the pieces on board
	SDL_Rect gPieceClip = {0, 0, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1};
	for (int i = 1; i < 23; i ++) {
		for(int j = 3; j < 13; j ++) {
			if(board[i][j] != FREE_BLOCK) {
				block[board[i][j]].Render(renderer, 
				BOARD_START_X + BLOCK_WIDTH * (j - 3), 
				BOARD_START_Y + BLOCK_HEIGHT * (i - 1), &gPieceClip);
			}
		}
	}
	
	// draw the current piece ghost
	DrawGhostPiece(renderer);
	// draw the current piece 
	currentPiece.DrawPiece(renderer, block);
	// draw the hold piece
	if(holdPieceType != NO_PIECE)
	holdPiece.DrawPiece(renderer, block);
	// draw the next 3 piece
	for(int i = 0; i < 3; i ++)
		nextPiece[i].DrawPiece(renderer, block);
}

void Board::TextureInit(SDL_Renderer* renderer){
	boardTex.LoadTextureFromFile("images/plain_play_field.png", renderer);
	playFieldBG.LoadTextureFromFile("images/play_field_bg.png", renderer);

	block[1].LoadTextureFromFile("images/block_cyan.png", renderer);
  block[2].LoadTextureFromFile("images/block_lime.png", renderer);
  block[3].LoadTextureFromFile("images/block_navy.png", renderer);
  block[4].LoadTextureFromFile("images/block_orange.png", renderer);
  block[5].LoadTextureFromFile("images/block_pink.png", renderer);
  block[6].LoadTextureFromFile("images/block_red.png", renderer);
  block[7].LoadTextureFromFile("images/block_yellow.png", renderer);
	block[8].LoadTextureFromFile("images/block_ghost.png", renderer);
}

Board::Board() {
	for (int i = 0; i < BOARD_HEIGHT; i++)
		for (int j = 0; j < BOARD_WIDTH; j++)
			board[i][j] = CYAN_BLOCK;
	for(int i = 0; i < 23; i ++)
		for(int j = 3; j < 13; j ++)
			board[i][j] = FREE_BLOCK;

	InitWallKick();

	pieceCount = 0;
  holdPieceType = NO_PIECE;
	FirstPieceBag();
	SecondPieceBag();
  currentPiece.Init(incomingPiece[pieceCount]);
	currentPieceGhost.Init(incomingPiece[pieceCount]);
	UpdateGhostPiece();
	for(int i = 0; i < 3; i ++){
		nextPiece[i].Init(incomingPiece[pieceCount + 1 + i]);
		nextPiece[i].xOffSet = 5 + 4 * i;
		nextPiece[i].yOffSet = 14;
	}
	holdThisTurn = 0;
}

Board::~Board() {
	boardTex.FreeTexture();
  playFieldBG.FreeTexture();
  for (int i = 0; i < TOTAL_BLOCK_COLOR; i ++)
    block[i].FreeTexture();
}

void Board::InitWallKick() {
  // JLSZT piece wall kick data
  // 0 -> R
  wallKickData[0][0][0][0][0] =  0, wallKickData[0][0][0][0][1] =  0;
  wallKickData[0][0][0][1][0] =  0, wallKickData[0][0][0][1][1] = -1;
  wallKickData[0][0][0][2][0] = -1, wallKickData[0][0][0][2][1] = -1;
  wallKickData[0][0][0][3][0] =  2, wallKickData[0][0][0][3][1] =  0;
  wallKickData[0][0][0][4][0] =  2, wallKickData[0][0][0][4][1] = -1;
  // R -> 0
  wallKickData[0][1][1][0][0] =  0, wallKickData[0][1][1][0][1] =  0;
  wallKickData[0][1][1][1][0] =  0, wallKickData[0][1][1][1][1] =  1;
  wallKickData[0][1][1][2][0] =  1, wallKickData[0][1][1][2][1] =  1;
  wallKickData[0][1][1][3][0] = -2, wallKickData[0][1][1][3][1] =  0;
  wallKickData[0][1][1][4][0] = -2, wallKickData[0][1][1][4][1] =  1;
  // R -> 2
  wallKickData[0][1][0][0][0] =  0, wallKickData[0][1][0][0][1] = 0;
  wallKickData[0][1][0][1][0] =  0, wallKickData[0][1][0][1][1] = 1;
  wallKickData[0][1][0][2][0] =  1, wallKickData[0][1][0][2][1] = 1;
  wallKickData[0][1][0][3][0] = -2, wallKickData[0][1][0][3][1] = 0;
  wallKickData[0][1][0][4][0] = -2, wallKickData[0][1][0][4][1] = 1;
  // 2 -> R
  wallKickData[0][2][1][0][0] =  0, wallKickData[0][2][1][0][1] =  0;
  wallKickData[0][2][1][1][0] =  0, wallKickData[0][2][1][1][1] = -1;
  wallKickData[0][2][1][2][0] = -1, wallKickData[0][2][1][2][1] = -1;
  wallKickData[0][2][1][3][0] =  2, wallKickData[0][2][1][3][1] =  0;
  wallKickData[0][2][1][4][0] =  2, wallKickData[0][2][1][4][1] = -1;
  // 2 -> L
  wallKickData[0][2][0][0][0] =  0, wallKickData[0][2][0][0][1] = 0;
  wallKickData[0][2][0][1][0] =  0, wallKickData[0][2][0][1][1] = 1;
  wallKickData[0][2][0][2][0] = -1, wallKickData[0][2][0][2][1] = 1;
  wallKickData[0][2][0][3][0] =  2, wallKickData[0][2][0][3][1] = 0;
  wallKickData[0][2][0][4][0] =  2, wallKickData[0][2][0][4][1] = 1;
  // L -> 2
  wallKickData[0][3][1][0][0] =  0, wallKickData[0][3][1][0][1] =  0;
  wallKickData[0][3][1][1][0] =  0, wallKickData[0][3][1][1][1] = -1;
  wallKickData[0][3][1][2][0] =  1, wallKickData[0][3][1][2][1] = -1;
  wallKickData[0][3][1][3][0] = -2, wallKickData[0][3][1][3][1] =  0;
  wallKickData[0][3][1][4][0] = -2, wallKickData[0][3][1][4][1] = -1;
  // L -> 0
  wallKickData[0][3][0][0][0] =  0, wallKickData[0][3][0][0][1] =  0;
  wallKickData[0][3][0][1][0] =  0, wallKickData[0][3][0][1][1] = -1;
  wallKickData[0][3][0][2][0] =  1, wallKickData[0][3][0][2][1] = -1;
  wallKickData[0][3][0][3][0] = -2, wallKickData[0][3][0][3][1] =  0;
  wallKickData[0][3][0][4][0] = -2, wallKickData[0][3][0][4][1] = -1;
  // 0 -> L
  wallKickData[0][0][1][0][0] =  0, wallKickData[0][0][1][0][1] = 0;
  wallKickData[0][0][1][1][0] =  0, wallKickData[0][0][1][1][1] = 1;
  wallKickData[0][0][1][2][0] = -1, wallKickData[0][0][1][2][1] = 1;
  wallKickData[0][0][1][3][0] =  2, wallKickData[0][0][1][3][1] = 0;
  wallKickData[0][0][1][4][0] =  2, wallKickData[0][0][1][4][1] = 1;
  
  // I piece wall kick data
  // 0 -> R
  wallKickData[1][0][0][0][0] =  0, wallKickData[1][0][0][0][1] =  0;
  wallKickData[1][0][0][1][0] =  0, wallKickData[1][0][0][1][1] = -2;
  wallKickData[1][0][0][2][0] =  0, wallKickData[1][0][0][2][1] =  1;
  wallKickData[1][0][0][3][0] =  1, wallKickData[1][0][0][3][1] = -2;
  wallKickData[1][0][0][4][0] = -2, wallKickData[1][0][0][4][1] =  1;
  // R -> 0
  wallKickData[1][1][1][0][0] =  0, wallKickData[1][1][1][0][1] =  0;
  wallKickData[1][1][1][1][0] =  0, wallKickData[1][1][1][1][1] =  2;
  wallKickData[1][1][1][2][0] =  0, wallKickData[1][1][1][2][1] = -1;
  wallKickData[1][1][1][3][0] = -1, wallKickData[1][1][1][3][1] =  2;
  wallKickData[1][1][1][4][0] =  2, wallKickData[1][1][1][4][1] = -1;
  // R -> 2 // fix y
  wallKickData[1][1][0][0][0] =  0, wallKickData[1][1][0][0][1] =  0;
  wallKickData[1][1][0][1][0] =  0, wallKickData[1][1][0][1][1] = -1;
  wallKickData[1][1][0][2][0] =  0, wallKickData[1][1][0][2][1] =  2;
  wallKickData[1][1][0][3][0] = -2, wallKickData[1][1][0][3][1] = -1;
  wallKickData[1][1][0][4][0] =  1, wallKickData[1][1][0][4][1] =  2;
  // 2 -> R
  wallKickData[1][2][1][0][0] =  0, wallKickData[1][2][1][0][1] =  0;
  wallKickData[1][2][1][1][0] =  0, wallKickData[1][2][1][1][1] =  1;
  wallKickData[1][2][1][2][0] =  0, wallKickData[1][2][1][2][1] = -2;
  wallKickData[1][2][1][3][0] =  2, wallKickData[1][2][1][3][1] =  1;
  wallKickData[1][2][1][4][0] = -1, wallKickData[1][2][1][4][1] = -2;
  // 2 -> L
  wallKickData[1][2][0][0][0] =  0, wallKickData[1][2][0][0][1] =  0;
  wallKickData[1][2][0][1][0] =  0, wallKickData[1][2][0][1][1] =  2;
  wallKickData[1][2][0][2][0] =  0, wallKickData[1][2][0][2][1] = -1;
  wallKickData[1][2][0][3][0] = -1, wallKickData[1][2][0][3][1] =  2;
  wallKickData[1][2][0][4][0] =  2, wallKickData[1][2][0][4][1] = -1;
  // L -> 2
  wallKickData[1][3][1][0][0] =  0, wallKickData[1][3][1][0][1] =  0;
  wallKickData[1][3][1][1][0] =  0, wallKickData[1][3][1][1][1] = -2;
  wallKickData[1][3][1][2][0] =  0, wallKickData[1][3][1][2][1] =  1;
  wallKickData[1][3][1][3][0] =  1, wallKickData[1][3][1][3][1] = -2;
  wallKickData[1][3][1][4][0] = -2, wallKickData[1][3][1][4][1] =  1;
  // L -> 0  
  wallKickData[1][3][0][0][0] =  0, wallKickData[1][3][0][0][1] =  0;
  wallKickData[1][3][0][1][0] =  0, wallKickData[1][3][0][1][1] =  1;
  wallKickData[1][3][0][2][0] =  0, wallKickData[1][3][0][2][1] = -2;
  wallKickData[1][3][0][3][0] =  2, wallKickData[1][3][0][3][1] =  1;
  wallKickData[1][3][0][4][0] = -1, wallKickData[1][3][0][4][1] = -2;
  // 0 -> L
  wallKickData[1][2][0][0][0] =  0, wallKickData[1][2][0][0][1] =  0;
  wallKickData[1][2][0][1][0] =  0, wallKickData[1][2][0][1][1] = -1;
  wallKickData[1][2][0][2][0] =  0, wallKickData[1][2][0][2][1] =  2;
  wallKickData[1][2][0][3][0] = -2, wallKickData[1][2][0][3][1] = -1;
  wallKickData[1][2][0][4][0] =  1, wallKickData[1][2][0][4][1] =  2;
}
