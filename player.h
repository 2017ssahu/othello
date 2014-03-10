#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <list>
<<<<<<< HEAD
=======

>>>>>>> 8a772be42ce16236728f3acdeea04565a61c1a77
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

private:
    Side mySide;
    Side opponentSide;
    Board* masterBoard;
    Move* randomMove();
<<<<<<< HEAD
    Move* simpleHeuristicMove();
=======
    std::list<Move*>* possibleMoves();
>>>>>>> 8a772be42ce16236728f3acdeea04565a61c1a77
};

#endif
