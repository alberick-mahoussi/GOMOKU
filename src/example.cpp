#include "pisqpipe.h"
#include <windows.h>

const char *infotext="name=\"Random\", author=\"Petr Lastovicka\", version=\"3.2\", country=\"Czech Republic\", www=\"https://plastovicka.github.io\"";

#define MAX_BOARD 100
int board[MAX_BOARD][MAX_BOARD];
static unsigned seed;
const int MAX_DEPTH = 3;


void brain_init() 
{
  if(width>MAX_BOARD || height>MAX_BOARD){
    width = height = 0;
    pipeOut("ERROR Maximal board size is %d", MAX_BOARD);
    return;
  }
  seed=start_time;
  pipeOut("OK");
}

void brain_restart()
{
  int x,y;
  for(x=0; x<width; x++){
    for(y=0; y<height; y++){
      board[x][y]=0;
    }
  }
  pipeOut("OK");
}

int isFree(int x, int y)
{
  return x>=0 && y>=0 && x<width && y<height && board[x][y]==0;
}

int min(int a, int b) {
  return (a < b) ? a : b;
}

int max(int a, int b) {
  return (a > b) ? a : b;
}

bool isGameOver() {
  // Implement logic to determine if the game has ended
  return false;
}

void brain_my(int x,int y)
{
  if(isFree(x,y)){
    board[x][y]=1;
  }else{
    pipeOut("ERROR my move [%d,%d]",x,y);
  }
}

void brain_opponents(int x,int y) 
{
  if(isFree(x,y)){
    board[x][y]=2;
  }else{
    pipeOut("ERROR opponents's move [%d,%d]",x,y);
  }
}
int evaluateBoard() {
  int score = 0;
  // Implement logic to evaluate the board state
  return score;
}

void brain_block(int x,int y)
{
  if(isFree(x,y)){
    board[x][y]=3;
  }else{
    pipeOut("ERROR winning move [%d,%d]",x,y);
  }
}

int brain_takeback(int x,int y)
{
  if(x>=0 && y>=0 && x<width && y<height && board[x][y]!=0){
    board[x][y]=0;
    return 0;
  }
  return 2;
}

unsigned rnd(unsigned n)
{
  seed=seed*367413989+174680251;
  return (unsigned)(UInt32x32To64(n,seed)>>32);
}

int minimax(int depth, bool isMaximizingPlayer, int alpha, int beta) {
  if (depth == MAX_DEPTH || isGameOver()) {
    return evaluateBoard();
  }

  if (isMaximizingPlayer) {
    int maxEval = INT_MIN;
    for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
          if (isFree(x, y)) {
              board[x][y] = 1; // AI makes a move
              int eval = minimax(depth + 1, false, alpha, beta);
              board[x][y] = 0; // Undo the move
              maxEval = max(maxEval, eval);
              alpha = max(alpha, eval);
              if (beta <= alpha) break;
          }
      }
    }
    return maxEval;
  } else {
    int minEval = INT_MAX;
    for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
          if (isFree(x, y)) {
              board[x][y] = 2; // Opponent's move
              int eval = minimax(depth + 1, true, alpha, beta);
              board[x][y] = 0; // Undo the move
              minEval = min(minEval, eval);
              beta = min(beta, eval);
              if (beta <= alpha) break;
          }
      }
    }
    return minEval;
  }
}

void brain_turn() {
  int bestScore = INT_MIN;
  int bestX = -1, bestY = -1;

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (isFree(x, y)) {
          board[x][y] = 1; // AI makes a move
          int score = minimax(0, false, INT_MIN, INT_MAX);
          board[x][y] = 0;

          if (score > bestScore) {
              bestScore = score;
              bestX = x;
              bestY = y;
          }
      }
    }
  }

  if (bestX != -1 && bestY != -1) {
    do_mymove(bestX, bestY); // Make the best move found
  }
}


void brain_end()
{
}

#ifdef DEBUG_EVAL
#include <windows.h>

void brain_eval(int x,int y)
{
  HDC dc;
  HWND wnd;
  RECT rc;
  char c;
  wnd=GetForegroundWindow();
  dc= GetDC(wnd);
  GetClientRect(wnd,&rc);
  c=(char)(board[x][y]+'0');
  TextOut(dc, rc.right-15, 3, &c, 1);
  ReleaseDC(wnd,dc);
}

#endif
