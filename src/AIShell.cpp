#include "AIShell.h"
#include <iostream>
#include <cstdlib>
#include <tuple>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>



AIShell::AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove)
{
	this->deadline=5000;
	this->numRows=numRows;
	this->numCols=numCols;
	this->gravityOn=gravityOn;
	this->gameState=gameState;
	this->lastMove=lastMove;
}


AIShell::~AIShell()
{

	//delete the gameState variable.
	for (int i =0; i<numCols; i++){
		delete [] gameState[i];
	}
	delete [] gameState;

}

std::vector<Move> AIShell::poss_moves(){
	std::vector<Move> possible;
	for (int col = 0; col<numCols; col++){
		for (int row = 0; row<numRows; row++){
			if (gameState[col][row] == NO_PIECE)
				possible.push_back(Move(col, row));
		}
	}
	return possible;
}

std::vector<Move> AIShell::poss_moves_gravity(){
	std::vector<Move> possible;
	for (int col = 0; col<numCols; col++){
		for (int row = 0; row<numRows; row++){
			if (gameState[col][row] == NO_PIECE){
			possible.push_back(Move(col, row));
			break;
			}
		}
	}
	return possible;
}

std::vector<Move> AIShell::change_move_order(std::vector<Move> poss_move, Move bestmove){
	std::vector<Move> new_poss_move;
	int position = 0;
	while (true){
		if (poss_move.at(position).col == bestmove.col && poss_move.at(position).row == bestmove.row){
			break;
		}
		else{
			position += 1;
		}
	}
	Move temp = poss_move.at(0);
	poss_move.at(0) = poss_move.at(position);
	poss_move.at(position) = temp;
	for (unsigned int i = 0; i < poss_move.size(); i++){
		new_poss_move.push_back(poss_move.at(i));
	}
	return new_poss_move;
}

std::tuple<int,int> AIShell::threat(int **board){
	std::tuple<int,int> most_potential = std::tuple<int,int>(-1,-1);
	for (int col = 0; col<numCols; col++)
	{
		for (int row = 0; row<numRows; row++)
		{
			if (col - 1 < 0 || board[col-1][row] != board[col][row]) { // horizontal
				int current_win = board[col][row];
				int count = 1;
				int count_of_same = (current_win != NO_PIECE) ? 1:0;
				std::tuple<int,int> if_only_one = std::tuple<int,int>(-1,-1);
				if (board[col][row] == NO_PIECE)
				{if_only_one = std::tuple<int,int>(col,row);}
				while (col + count < numCols) {
					if (current_win== NO_PIECE && board[col][row] != board[col+count][row])
					{current_win = board[col+count][row];}
					else if (current_win != NO_PIECE && board[col+count][row]!= NO_PIECE && current_win != board[col+count][row])
					{break;}
					if (board[col+count][row] == current_win && current_win != NO_PIECE)
					{count_of_same += 1;}
					else if (board[col+count][row] == NO_PIECE)
					{if_only_one = std::tuple<int,int>(col+count,row);}
					++count;
					if (count >= k && current_win != NO_PIECE) {
						if (count_of_same == k-1 && current_win == AI_PIECE)
						{return if_only_one;}
						if (count_of_same == k-1 && current_win == HUMAN_PIECE)
						{most_potential = if_only_one;
						break;}
					}
				}
			}
			if (col - 1 < 0 || row - 1 < 0 || board[col - 1][row - 1] != board[col][row]) { // diagonal
				int current_win = board[col][row];
				int count = 1;
				int count_of_same = (current_win != NO_PIECE) ? 1:0;
				std::tuple<int,int> if_only_one = std::tuple<int,int>(-1,-1);
				if (board[col][row] == NO_PIECE)
				{if_only_one = std::tuple<int,int>(col,row);}
				while (col + count < numCols && row + count < numRows) {
					if (current_win== NO_PIECE && board[col][row] != board[col+count][row+count])
					{current_win = board[col+count][row+count];}
					else if (current_win != NO_PIECE && board[col+count][row+count]!= NO_PIECE && current_win != board[col+count][row+count])
					{break;}
					if (board[col+count][row+count] == current_win && current_win != NO_PIECE)
					{count_of_same += 1;}
					else if (board[col+count][row+count] == NO_PIECE)
					{if_only_one = std::tuple<int,int>(col+count,row+count);}
					++count;
					if (count >= k && current_win != NO_PIECE) {

						if (count_of_same == k-1 && current_win == AI_PIECE)
						{return if_only_one;}
						if (count_of_same == k-1 && current_win == HUMAN_PIECE)
						{most_potential = if_only_one;
						break;}
					}
				}
			}
			if (col - 1 < 0 || row + 1 >= numRows || board[col - 1][row + 1] != board[col][row]) { // diagonal
				int current_win = board[col][row];
				int count = 1;
				int count_of_same = (current_win != NO_PIECE) ? 1:0;
				std::tuple<int,int> if_only_one = std::tuple<int,int>(-1,-1);
				if (board[col][row] == NO_PIECE)
				{if_only_one = std::tuple<int,int>(col,row);}
				while (col + count < numCols && row - count >= 0) {
					if (current_win== NO_PIECE && board[col][row] != board[col+count][row-count])
					{current_win = board[col+count][row-count];}
					else if (current_win != NO_PIECE && board[col+count][row-count]!= NO_PIECE && current_win != board[col+count][row-count])
					{break;}
					if (board[col+count][row-count] == current_win && current_win != NO_PIECE)
					{count_of_same += 1;}
					else if (board[col+count][row-count] == NO_PIECE)
					{if_only_one = std::tuple<int,int>(col+count,row-count);}
					++count;
					if (count >= k && current_win != NO_PIECE) {

						if (count_of_same == k-1 && current_win == AI_PIECE)
						{return if_only_one;}
						if (count_of_same == k-1 && current_win == HUMAN_PIECE)
						{most_potential = if_only_one;
						break;}
					}
				}
			}
			if (row - 1 < 0 || board[col][row-1] != board[col][row]) { // vertical
				int current_win = board[col][row];
				int count = 1;
				int count_of_same = (current_win != NO_PIECE) ? 1:0;
				std::tuple<int,int> if_only_one = std::tuple<int,int>(-1,-1);
				if (board[col][row] == NO_PIECE)
				{if_only_one = std::tuple<int,int>(col,row);}
				while (row + count < numRows) {
					if (current_win== NO_PIECE && board[col][row] != board[col][row+count])
					{current_win = board[col][row+count];}
					else if (current_win != NO_PIECE && board[col][row+count]!= NO_PIECE && current_win != board[col][row+count])
					{break;}
					if (board[col][row+count] == current_win && current_win != NO_PIECE)
					{count_of_same += 1;}
					else if (board[col][row+count] == NO_PIECE)
					{if_only_one = std::tuple<int,int>(col,row+count);}
					++count;
					if (count >= k && current_win != NO_PIECE) {

							if (count_of_same == k-1 && current_win == AI_PIECE)
							{return if_only_one;}
							if (count_of_same == k-1 && current_win == HUMAN_PIECE)
							{most_potential = if_only_one;
							break;}
					}
 					}
				}
			}
		}
	return most_potential;
}

int AIShell::heuristic(int **board){
	int AIwins = 0;
	int oppwins = 0;
	for (int col = 0; col<numCols; col++)
	{
		for (int row = 0; row<numRows; row++)
		{
			if (col - 1 < 0 || board[col-1][row] != board[col][row]) { // horizontal
				int current_win = board[col][row];
				int count = 1;
				int count_of_same = (current_win != NO_PIECE) ? 1:0;
				while (col + count < numCols) {
					if (current_win== NO_PIECE && board[col][row] != board[col+count][row])
					{current_win = board[col+count][row];}
					else if (current_win != NO_PIECE && board[col+count][row]!= NO_PIECE && current_win != board[col+count][row])
					{break;}
					if (board[col][row+count] == current_win && current_win != NO_PIECE)
					{count_of_same += 1;}
					++count;
					if (count >= k) {
						if (current_win == AI_PIECE)
						{AIwins+=count_of_same;
						break;}
						else if(current_win == HUMAN_PIECE)
						{oppwins+=count_of_same;
						break;}
					}
				}
			}

			if (col - 1 < 0 || row - 1 < 0 || board[col - 1][row - 1] != board[col][row]) { // diagonal
				int current_win = board[col][row];
				int count = 1;
				int count_of_same = (current_win != NO_PIECE) ? 1:0;
				while (col + count < numCols && row + count < numRows) {
					if (current_win== NO_PIECE && board[col][row] != board[col+count][row+count])
					{current_win = board[col+count][row+count];}
					else if (current_win != NO_PIECE && board[col+count][row+count]!= NO_PIECE && current_win != board[col+count][row+count])
					{break;}
					if (board[col+count][row+count] == current_win && current_win != NO_PIECE)
					{count_of_same += 1;}
					++count;
					if (count >= k) {
						if (current_win == AI_PIECE)
						{AIwins+=count_of_same;
						break;}
						else if(current_win == HUMAN_PIECE)
						{oppwins+=count_of_same;
						break;}
					}
				}
			}

			if (col - 1 < 0 || row + 1 >= numRows || board[col - 1][row + 1] != board[col][row]) { // diagonal
				int current_win = board[col][row];
				int count = 1;
				int count_of_same = (current_win != NO_PIECE) ? 1:0;
				while (col + count < numCols && row - count >= 0) {
					if (current_win== NO_PIECE && board[col][row] != board[col+count][row-count])
					{current_win = board[col+count][row-count];}
					else if (current_win != NO_PIECE && board[col+count][row-count]!= NO_PIECE && current_win != board[col+count][row-count])
					{break;}
					if (board[col+count][row-count] == current_win && current_win != NO_PIECE)
					{count_of_same += 1;}
					++count;
					if (count >= k) {
						if (current_win == AI_PIECE)
						{AIwins+=count_of_same;
						break;}
						else if(current_win == HUMAN_PIECE)
						{oppwins+=count_of_same;
						break;}
					}
				}
			}
			if (row - 1 < 0 || board[col][row-1] != board[col][row]) { // vertical
				int current_win = board[col][row];
				int count = 1;
				int count_of_same = (current_win != NO_PIECE) ? 1:0;
				while (row + count < numRows) {
					if (current_win== NO_PIECE && board[col][row] != board[col][row+count])
					{current_win = board[col][row+count];}
					else if (current_win != NO_PIECE && board[col][row+count]!= NO_PIECE && current_win != board[col][row+count])
					{break;}
					if (board[col][row+count] == current_win && current_win != NO_PIECE)
					{count_of_same += 1;}
					++count;
 					if (count >= k) {
 						if (current_win == AI_PIECE)
						{AIwins+=count_of_same;
						break;}
 						else if(current_win == HUMAN_PIECE)
 						{oppwins+=count_of_same;
 						break;}
 					}
				}
			}
		}
	}
	return (AIwins - oppwins);
}

std::tuple<int, int, int> AIShell::mini_max(int player, int depth, int alpha, int beta, std::vector<Move> possible_moves, std::chrono::time_point<std::chrono::system_clock> start){
	int bestmove = (player == AI_PIECE) ? -9999:9999;
	int currentscore;
	int bestcol = possible_moves.at(0).col;
	int bestrow = possible_moves.at(0).row;

	if (possible_moves.empty() || depth == 0){
		bestmove = heuristic(gameState);
	}
	else
	{
		for (Move m: possible_moves){
			std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
				std::chrono::duration<double> elapsed_seconds = end-start;
						if (elapsed_seconds.count()*1000 >= deadline){
							return std::tuple<int,int,int>(bestcol, bestrow, 99999999);}
			gameState[m.col][m.row] = player;
			if (player == 1){
				currentscore = std::get<2>(mini_max(-player, depth-1, alpha, beta,possible_moves, start));
				if (currentscore == 99999999){
					gameState[m.col][m.row] = NO_PIECE;
					return std::tuple<int,int,int>(bestcol, bestrow, 99999999);
				}
				if (currentscore > bestmove){
					bestmove = currentscore;
					alpha = bestmove;
					bestcol = m.col;
					bestrow = m.row;
					if (bestmove >= beta){
						gameState[m.col][m.row] = NO_PIECE;
						return std::tuple<int,int,int>(bestcol, bestrow, 9999);
					}
				}
			}
			else{
				currentscore = std::get<2>(mini_max(-player, depth-1, alpha, beta, possible_moves, start));
				if (currentscore == 99999999){
					gameState[m.col][m.row] = NO_PIECE;
					return std::tuple<int,int,int>(bestcol, bestrow, 99999999);
				}
				if (currentscore < bestmove){
					bestmove = currentscore;
					beta = bestmove;
					bestcol = m.col;
					bestrow = m.row;
					if (alpha >= bestmove){
						gameState[m.col][m.row] = NO_PIECE;
						return std::tuple<int,int,int>(bestcol, bestrow, -9999);
					}
				}
			}
			gameState[m.col][m.row] = NO_PIECE;
		}
	}
	return std::tuple<int,int,int>(bestcol, bestrow, bestmove);
}

std::tuple<int, int, int> AIShell::iter_deep(int player, int alpha, int beta){
	std::vector<Move> possible_moves;
	if (gravityOn)
		possible_moves = poss_moves_gravity();
	else
		possible_moves = poss_moves();
	std::chrono::time_point<std::chrono::system_clock> start, end;
	int depthtosearch = 0;
	std::tuple<int,int,int> bestmove, prevmove;
	start = std::chrono::system_clock::now();
	while (true){
		depthtosearch += 1;
		prevmove = bestmove;
		bestmove = mini_max(player, depthtosearch, alpha, beta, possible_moves, start);
		if (std::get<2>(bestmove) == 99999999){
			return prevmove;
		}
		Move temp(std::get<0>(bestmove), std::get<1>(bestmove));
		possible_moves = change_move_order(possible_moves, temp);
		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end-start;
		if (elapsed_seconds.count()*1000 >= deadline){
			break;
		}
	}
	return bestmove;
}

Move AIShell::makeMove(){
	std::tuple<int,int> potential = threat(gameState);
	if (std::get<0>(potential) != -1)
	{
		Move m(std::get<0>(potential), std::get<1>(potential));
		return m;
	}
	else
	{
		std::tuple<int,int,int> bestmove;
	bestmove = iter_deep(AI_PIECE, -9999, 9999);
	Move m(std::get<0>(bestmove), std::get<1>(bestmove));
	return m;
	}

}
