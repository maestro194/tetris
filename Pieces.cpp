#include "Pieces.h"

Piece::Piece() {

}

void Piece::Init(int pieceNumber) {
  rotation = 0;
  for(int i = 0; i < 4; i ++)
    for(int j = 0; j < 4; j ++)
      for(int k = 0; k < 4; k ++)
        shape[i][j][k] = ' ';
  xOffSet = 1;
  yOffSet = 6;
  switch (pieceNumber)
  {
  case O_PIECE:
    pieceType = O_PIECE;
    shape[0][0][1] = shape[0][0][2] = shape[0][1][1] = shape[0][1][2] = '*';
    shape[1][0][1] = shape[1][0][2] = shape[1][1][1] = shape[1][1][2] = '*';
    shape[2][0][1] = shape[2][0][2] = shape[2][1][1] = shape[2][1][2] = '*';
    shape[3][0][1] = shape[3][0][2] = shape[3][1][1] = shape[3][1][2] = '*';
    break;
  case L_PIECE:
    pieceType = L_PIECE;
    shape[0][0][2] = shape[0][1][0] = shape[0][1][1] = shape[0][1][2] = '*';
    shape[1][0][1] = shape[1][1][1] = shape[1][2][1] = shape[1][2][2] = '*';
    shape[2][1][0] = shape[2][1][1] = shape[2][1][2] = shape[2][2][0] = '*';
    shape[3][0][0] = shape[3][0][1] = shape[3][1][1] = shape[3][2][1] = '*';
    break;
  case J_PIECE:
    pieceType = J_PIECE;
    shape[0][0][0] = shape[0][1][0] = shape[0][1][1] = shape[0][1][2] = '*';
    shape[1][0][1] = shape[1][0][2] = shape[1][1][1] = shape[1][2][1] = '*';
    shape[2][1][0] = shape[2][1][1] = shape[2][1][2] = shape[2][2][2] = '*';
    shape[3][0][1] = shape[3][1][1] = shape[3][2][0] = shape[3][2][1] = '*';
    break;
  case I_PIECE:
    pieceType = I_PIECE;
    shape[0][1][0] = shape[0][1][1] = shape[0][1][2] = shape[0][1][3] = '*';
    shape[1][0][2] = shape[1][1][2] = shape[1][2][2] = shape[1][3][2] = '*';
    shape[2][2][0] = shape[2][2][1] = shape[2][2][2] = shape[2][2][3] = '*';
    shape[3][1][0] = shape[3][1][1] = shape[3][1][2] = shape[3][1][3] = '*';
    break;
  case Z_PIECE:
    pieceType = Z_PIECE;
    shape[0][0][0] = shape[0][0][1] = shape[0][1][1] = shape[0][1][2] = '*';
    shape[1][0][2] = shape[1][1][1] = shape[1][1][2] = shape[1][2][1] = '*';
    shape[2][1][0] = shape[2][1][1] = shape[2][2][1] = shape[2][2][2] = '*';
    shape[3][0][1] = shape[3][1][0] = shape[3][1][1] = shape[3][2][0] = '*';
    break;
  case S_PIECE:
    pieceType = S_PIECE;
    shape[0][0][1] = shape[0][0][2] = shape[0][1][0] = shape[0][1][1] = '*';
    shape[1][0][1] = shape[1][1][1] = shape[1][1][2] = shape[1][2][2] = '*';
    shape[2][1][1] = shape[2][1][2] = shape[2][2][0] = shape[2][2][1] = '*';
    shape[3][0][0] = shape[3][1][0] = shape[3][1][1] = shape[3][2][1] = '*';
    break;
  case T_PIECE:
    pieceType = T_PIECE;
    shape[0][0][1] = shape[0][1][0] = shape[0][1][1] = shape[0][1][2] = '*';
    shape[1][0][1] = shape[1][1][1] = shape[1][1][2] = shape[1][2][1] = '*';
    shape[2][1][0] = shape[2][1][1] = shape[2][1][2] = shape[2][2][1] = '*';
    shape[3][0][1] = shape[3][1][0] = shape[3][1][1] = shape[3][2][1] = '*';
    break;
  }
}

void Piece::PieceLeftMove(){
  yOffSet --;
}

void Piece::PieceRightMove(){
  yOffSet ++;
}

void Piece::PieceUpMove(){
  xOffSet --;
}

void Piece::PieceDownMove(){
  xOffSet ++;
}

void Piece::PieceDropMove(){
  
}

void Piece::PieceCWRotateMove(){
  rotation = (rotation + 1) % 4;
}

void Piece::PieceCCWRotateMove(){
  rotation = (rotation + 3) % 4;
}

void Piece::PieceFlipMove(){
  rotation = (rotation + 2) % 4;
}

void Piece::DrawPiece(SDL_Renderer* renderer, Texture gBlock[]) {
  SDL_Rect gPieceClip = {0, 0, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1};
  for(int i = 0; i < 4; i ++)
    for(int j = 0; j < 4; j ++)
      if(shape[rotation][i][j] == '*')
        gBlock[pieceType].Render(renderer, 
        BOARD_START_X + BLOCK_WIDTH * (j - 3 + yOffSet),
        BOARD_START_Y + BLOCK_HEIGHT * (i - 1 + xOffSet),
        &gPieceClip);
}

Piece::~Piece() {

}