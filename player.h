#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <cstdio>
#include "common.h"
#include "board.h"
#include <list>
using namespace std;

class Player {
    
public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    void setBoard(Board* board);

private:
    Side mySide;
    Side opponentSide;
    Board* masterBoard;
      
    std::list<Move*>* possibleMoves(Board* tempBoard,Side side);
    double heuristic(Side side,Board* originalBoard);
    void printTree(std::list<DecisionTreeNode*>* childrenList);
    Move* miniMaxMove(int depth);
    double miniMaxMove(int depth, Board* previousBoard,Side side, DecisionTreeNode* parentNode,double alpha,double beta);
    Side flip(Side side);
    DecisionTreeNode* findMax (std::list<DecisionTreeNode*>* list);
    DecisionTreeNode* findMin (std::list<DecisionTreeNode*>* list);
    double max(double one, double two);
    double min(double one, double two);	

};

#endif
