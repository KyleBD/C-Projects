#ifndef GameState_h
#define GameState_h

#include "globals.h"

class GameState {
 private:
  int selectedRow;  
  int selectedColumn;  
  bool moveValid; 
                  
  bool
      gameOver;
  bool turn;
  int winner;
  int gameBoard[boardSize][boardSize];

 public:
  GameState();
  
  int get_selectedRow();

  int get_selectedColumn();

  void set_selectedRow(int value);

  void set_selectedColumn(int value);

  bool get_moveValid();

  void set_moveValid(bool value);

  bool get_gameOver();

  void set_gameOver(bool value);

  bool get_turn();

  void set_turn(bool value);

  int get_winner();

  void set_winner(int value);

  int get_gameBoard(int row, int col);

  void set_gameBoard(int row, int col, int value);
};

#endif /* GameState_h */
