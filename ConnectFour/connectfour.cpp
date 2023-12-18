#include <iostream>
#include <string>
using namespace std;

#include "GameState.h"
#include "globals.h"

int main() {
  int winner = 0;

  const int numOfRounds = 3;
  GameState game_state[numOfRounds];

  cout << "Game 1" << endl;
  int col;
  int round = 0;
  while (!game_state[round].get_gameOver()) {
    cout << "Enter column to place piece: ";
    cin >> col;

    if( cin.eof() ) {
      cerr << endl << "Game ended by user." << endl;
      exit( 0 ) ;
    }
    if( cin.fail() ) {
      cin.clear() ;
      cin.ignore( 1000, '\n' ) ;
      col = -1; // giving col invalid value so it will be handled as invalid input below
    }
    //create the board
    //Set the input
    game_state[round].set_selectedColumn(col);
    game_state[round].set_selectedRow(col);
    if(!game_state[round].get_moveValid()){
      cout << "Invalid column!" << endl;
      //reset the flag 
      game_state[round].set_moveValid(true);
    }
    else{
      cout << "column chosen: " << col << endl;
      playMove(game_state[round]);
    }

    if(game_state[round].get_gameOver() == true){
      winner = game_state[round].get_winner();
      if(winner == 1){
        cout << "R won this round!" << endl;
      }
      if(winner == -1){
        cout << "Y won this round!" << endl;

      }
      }
  }
  cout << "Game 2" << endl;
  // 2nd Round
  round = 1;
  while (!game_state[round].get_gameOver()) {
    cout << "Enter column to place piece: ";
    cin >> col;
 
    if( cin.eof() ) {
      cerr << endl << "Game ended by user." << endl;
      exit( 0 ) ;
    }
    if( cin.fail() ) {
      cin.clear() ;
      cin.ignore( 1000, '\n' ) ;
      col = -1; // giving col invalid value so it will be handled as invalid input below
    }
    //create the board
    //Set the input
    game_state[round].set_selectedColumn(col);
    game_state[round].set_selectedRow(col);
    if(!game_state[round].get_moveValid()){
      cout << "Invalid column!" << endl;
      //reset the flag 
      game_state[round].set_moveValid(true);
    }
    else{
      cout << "column chosen: " << col << endl;
      playMove(game_state[round]);
    }
    // Check validity of input and if not valid, handle accordingly
    if(game_state[round].get_gameOver() == true){
      if(winner == game_state[round].get_winner()){
        if(winner == 1){
          cout << "R won this round!" << endl;
          cout << "R won the match!" << endl;
          exit ( 0 );
        }
        if(winner == -1){
          cout << "Y won this round!" << endl;
          cout << "Y won the match!" << endl;
          exit ( 0 );
        }
      }
      if(game_state[round].get_winner() == -1){
        cout << "Y won this round!" << endl;
      }
      if(game_state[round].get_winner() == 1){
        cout << "R won this round!" << endl;
      }

      
    }
    
  }
  //3rd round
  round = 2;
  cout << "Game 3" << endl;
  while (!game_state[round].get_gameOver()) {
    cout << "Enter column to place piece: ";
    cin >> col;

    if( cin.eof() ) {
      cerr << endl << "Game ended by user." << endl ;
      exit( 0 ) ;
    }
    if( cin.fail() ) {
      cin.clear() ;
      cin.ignore( 1000, '\n' ) ;
      col = -1; // giving col invalid value so it will be handled as invalid input below
    }
    //create the board
    //Set the input
    game_state[round].set_selectedColumn(col);
    game_state[round].set_selectedRow(col);
    if(!game_state[round].get_moveValid()){
      cout << "Invalid column!" << endl;
      //reset the flag 
      game_state[round].set_moveValid(true);
    }
    else{
      cout << "column chosen: " << col << endl;
      playMove(game_state[round]);
    }
    // Check validity of input and if not valid, handle accordingly
    if(game_state[round].get_gameOver() == true){
      winner = game_state[round].get_winner();
      if(winner == 1){
        cout << "R won this round!" << endl;
        cout << "R won the match" << endl;
        exit (0);
      }
      if(winner == -1){
        cout << "Y won this round!" << endl;
        cout << "Y won the match!" << endl;
        exit(0);
      }
      }
    }
 
}
