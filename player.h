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

private:
    int temp;
    Side mySide;
    Side opponentSide;
    Board* masterBoard;
    Move* randomMove();
    Move* simpleHeuristicMove();
    std::list<Move*>* possibleMoves(Board* tempBoard,Side side);
    void heuristic(Move* move,Side side,Board* originalBoard);
    Move* miniMaxMove(int depth);
    DecisionTreeNode* findMin(std::list<DecisionTreeNode*>* list);
    DecisionTreeNode* findMax(std::list<DecisionTreeNode*>* list);
};

#endif
