#include "GameState.h"
#include "globals.h"

#include <iostream>
#include <string>
using namespace std;

GameState::GameState(){
  selectedRow = 0;
  selectedColumn = 0;
  moveValid = true;
  gameOver = false;
  turn = true;
  winner = Empty;

  for(int i = 0; i<boardSize; i++){
    for(int z = 0; z< boardSize; z++){
      gameBoard[i][z] = Empty;
    }
  }
}

// Note... WILL TAKE IN THE VALUE OF THE SELECTED COL
void GameState::set_selectedRow(int value){
  for(int i = 0; i<boardSize; i++){
    if(gameBoard[i][selectedColumn] == Empty){
      selectedRow = i;
      return;
    }
  }
  set_moveValid(false);
  return;
}

void GameState::set_selectedColumn(int value){
  if(value<0 || value > 6){
    set_moveValid(false);
    return;
  }
  else{
    selectedColumn = value;
    return;
  }
}

int GameState::get_selectedRow(){
    return selectedRow;

}

int GameState::get_selectedColumn(){
    return selectedColumn;
}

bool GameState::get_moveValid(){
    return moveValid;
}

void GameState::set_moveValid(bool value){
    moveValid = value;
    return;
}

bool GameState::get_gameOver(){
    return gameOver;
}

void GameState::set_gameOver(bool value){
    gameOver = value;
}

bool GameState::get_turn(){
    return turn;
}

void GameState::set_turn(bool value){
    if(value == true){
        turn = false;
        return;
    }
    if(value == false){
        turn = true;
        return;
    }
}

int GameState::get_winner(){
    return winner;
}

void GameState::set_winner(int value){
    winner = value;
}
  
int GameState::get_gameBoard(int row, int col){
    if((row > boardSize-1) || (row  < 0) || (col > boardSize -1) || (col < 0)){
        return Empty;
    }
    return gameBoard[row][col];
}

//Evertime a piece is set check in all directions to see if that piece gives a connect four
void GameState::set_gameBoard(int row, int col, int value){

  gameBoard[row][col] = value;
  return;
}

