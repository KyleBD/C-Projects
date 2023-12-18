//
//  GameState.cpp
//  Connect Four Game
//
//  Created by Tarek Abdelrahman on 2019-06-07.
//  Modified by Tarek Abdelrahman on 2020-09-17.
//  Modified by Salma Emara and Ethan Hugh on 2023-09-06.
//  Copyright © 2019-2020 Tarek Abdelrahman. All rights reserved.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//

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
  //If the program reaches this point then the move will be not valid as the row is full
  set_moveValid(false);
  return;
}

  // Set the selected column in the range (in the range 0..boardSize - 1)
  // An out of range value is ignored and the function just returns
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

  // Return the selected column (in the range 0..boardSize - 1)
int GameState::get_selectedColumn(){
    return selectedColumn;
}


  // Get the moveValid value
bool GameState::get_moveValid(){
    return moveValid;
}

  // Set the moveValid variable to value
void GameState::set_moveValid(bool value){
    moveValid = value;
    return;
}

  // Get the gameOver value
bool GameState::get_gameOver(){
    return gameOver;
}

  // Set the gameOver variable to value
void GameState::set_gameOver(bool value){
    gameOver = value;
}

  // Get the value of turn
bool GameState::get_turn(){
    return turn;
}

  // Set the value of turn
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

  // Get the value of winner
int GameState::get_winner(){
    return winner;
}

  // Set the value of winner;
void GameState::set_winner(int value){
    winner = value;
}
  
  // Get the game board value at the board location at row and col
  // This method checks that row, col and value are in range/valid
  // and if not it returns Empty
int GameState::get_gameBoard(int row, int col){
    if((row > boardSize-1) || (row  < 0) || (col > boardSize -1) || (col < 0)){
        return Empty;
    }
    //cout << gameBoard[row][col] << endl;
    //cout << row << col;
    return gameBoard[row][col];
}

//Evertime a piece is set check in all directions to see if that piece gives a connect four
void GameState::set_gameBoard(int row, int col, int value){

  gameBoard[row][col] = value;
  return;
}

