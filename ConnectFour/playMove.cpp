#include "GameState.h"
#include "globals.h"

#include <iostream>
#include <string>

using namespace std;
bool in_bounds(int row, int col){
  if( (row < 0) || (row > 6) || (col < 0) || (col > 6)){
    return false;
  }
  else{
    return true;
  }
}
bool check_direction(int row, int col, int value, int delta_x, int delta_y, GameState game_state){
  int counter = 1;
  for(int i = 1; i<4; i++){
    int cur_row = row + delta_x*i;
    int cur_col = col + delta_y*i;
    if((game_state.get_gameBoard(cur_row, cur_col) == value) && in_bounds(cur_row, cur_col)){
      counter++;
    }
    if(counter == 4){
      return true;
    }
  }
  return false;
}
bool check_winner(int row, int col, bool value, GameState game_state){
  //We want to check in 8 different directions to see if there is a winner
  //There are 3 case
  //1. We go out of bounds
  //2. We reach an empty character || oposite colour character
  //3. We count 4 in a row before either of the previous two.
  int player;
  if(value == true){
    player = R;
  }
  else{
    player = Y;
  }
  //N
  if(check_direction(row, col, player, 0, 1, game_state)){
    return true;
  }
  //E
  if(check_direction(row, col, player, 1, 0, game_state)){
    return true;
  }
  //S
  if(check_direction(row, col, player, 0, -1, game_state)){
    return true;
  }
  //W
  if(check_direction(row, col, player, -1, 0, game_state)){
    return true;
  }
  //NE
  if(check_direction(row, col, player, 1, 1, game_state)){
    return true;
  }
  //SE
  if(check_direction(row, col, player, 1, -1, game_state)){
    return true;
  }
  //SW
  if(check_direction(row, col, player, -1, -1, game_state)){
    return true;
  }
  //NW
  if(check_direction(row, col, player, -1, 1, game_state)){
    return true;
  }
  return false;
}

void print_board(GameState game_state){
  for(int i = boardSize-1; i>=0; i--){
    for(int j = 0; j<boardSize; j++){
      if(game_state.get_gameBoard(i,j) == R){
        cout << 'R';
      }
      if(game_state.get_gameBoard(i,j) == Y){
        cout << 'Y';
      }
      if(game_state.get_gameBoard(i,j) == Empty){
        cout << '_';
      }
    }
    cout << endl;
  }
  return;
}

void playMove(GameState& game_state) {
  bool temp_player = game_state.get_turn();
  int cur_player;
  if (temp_player == true){
    cur_player = R;
  }
  if (temp_player == false){
    cur_player = Y;
  }
  int row = game_state.get_selectedRow();
  int col = game_state.get_selectedColumn();
  game_state.set_gameBoard(row, col, cur_player);

  if(in_bounds(row,col) && game_state.get_moveValid()){
    // First Check if there is a winner
    if(check_winner(row, col, game_state.get_turn(), game_state)){
      //change winner
      if(cur_player == game_state.get_winner()){
        game_state.set_gameOver(true);
      }
      game_state.set_winner(cur_player);
      //change gameover
      game_state.set_gameOver(true);
      // output the current game board
      print_board(game_state);

      return;
    }
    else{
      //change turn if valid move, no winner no game over
      game_state.set_turn(game_state.get_turn());
      //output the current game board
      print_board(game_state);
      return;
    }
  }
}




