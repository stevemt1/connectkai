#ifndef AISHELL_H
#define AISHELL_H
#pragma once
#include "Move.h"
#include <iostream>
#include <cstdlib>
#include <tuple>
#include <vector>
#include <algorithm>
#include <chrono>


// A new AIShell will be created for every move request.
class AIShell{

public:
	//these represent the values for each piece type.
	static const int AI_PIECE=1;
	static const int HUMAN_PIECE = -1;
	static const int NO_PIECE=0;


private:
 	 //Do not alter the values of numRows or numcols.
 	 //they are used for deallocating the gameState variable.
 	 int numRows; //the total number of rows in the game state.
 	 int numCols; //the total number of columns in the game state.
 	 int **gameState; //a pointer to a two-dimensional array representing the game state.
 	 bool gravityOn; //this will be true if gravity is turned on. It will be false if gravity is turned off.
 	 Move lastMove; //this is the move made last by your opponent. If your opponent has not made a move yet (you move first) then this move will hold the value (-1, -1) instead.



public:
 	 int deadline; //this is how many milliseconds the AI has to make move.
 	 int k;        // k is the number of pieces a player must get in a row/column/diagonal to win the game. IE in connect 4, this variable would be 4
 	 std::vector<Move> poss_moves();
 	 std::vector<Move> poss_moves_gravity();
 	 std::vector<Move> change_move_order(std::vector<Move> poss_move, Move bestmove);
 	 std::tuple<int, int> threat(int **board);
 	 int heuristic(int **board);
 	 std::tuple<int, int, int> mini_max(int player, int depth, int alpha, int beta, std::vector<Move> possible_moves, std::chrono::time_point<std::chrono::system_clock> start);
 	 std::tuple<int, int, int> iter_deep(int player, int alpha, int beta);
 	 AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove);
 	 ~AIShell();
 	 Move makeMove();
};

#endif //AISHELL_H
