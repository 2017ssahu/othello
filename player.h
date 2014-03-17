#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <cstdio>
#include "common.h"
#include "board.h"
#include <list>
using namespace std;

int weights[64] = {4,-3,2,2,2,2,-3,4,
-3,-4,-1,-1,-1,-1,-4,-3,
2,-1,1,0,0,1,-1,2,
2,-1,0,1,1,0,-1,2,
2,-1,0,1,1,0,-1,2,
2,-1,1,0,0,1,-1,2,
-3,-4,-1,-1,-1,-1,-4,-3,
4,-3,2,2,2,2,-3,4}; 

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
      

    Move* randomMove();
    Move* simpleHeuristicMove();
    std::list<Move*>* possibleMoves(Board* tempBoard,Side side);
    double heuristic(Side side,Board* originalBoard);
    void printTree(std::list<DecisionTreeNode*>* childrenList);
    Move* miniMaxMove(int depth);
    void miniMaxMove(int depth, Board* previousBoard,Side side, DecisionTreeNode* parentNode);
    Side flip(Side side);
    DecisionTreeNode* findMax (std::list<DecisionTreeNode*>* list);
    DecisionTreeNode* findMin (std::list<DecisionTreeNode*>* list);
};

#endif
