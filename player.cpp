#include "player.h"
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
	this->mySide = side;
    this->opponentSide = BLACK;
     
    this->opponentSide = BLACK; 
    
    if(side == BLACK)
    {
        opponentSide = WHITE;
    }

    masterBoard = new Board();

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
    //Process opponent's move
    masterBoard->doMove(opponentsMove, opponentSide);
    if (opponentsMove != NULL)
    {	
		masterBoard->doMove(opponentsMove,opponentSide);
	}
	
    //Determine move

    //return randomMove();
    //Determine move 
    return simpleHeuristicMove();
}

/**
 * Chooses a pseudo-random move by iterating through the board in order
 * and selecting the first valid move.
 * 
 */
Move* Player::randomMove()
{
	Move* randMove = new Move(0,0);
	
    for (int i = 0; i < 8; i++)
    {
		for (int j = 0; j < 8; j++)
		{
			randMove->setX(i);
			randMove->setY(j);
			
			if (masterBoard->checkMove(randMove,mySide))
			{
				masterBoard->doMove(randMove,mySide);
				return randMove;
			}
		}
	}	
	
	return NULL;
}

Move* Player::simpleHeuristicMove()
{
	std::list<Move*>* moveList = possibleMoves();
	
	for (std::list<Move*>::iterator i= moveList->begin(); i != moveList->end(); ++i)
	{
		heuristic(*i,mySide);	//assigns a heuristic score to each move
	}
	
	Move* bestMove = *(moveList->begin()); //Set bestMove to the first move initially
	
	for (std::list<Move*>::iterator i= moveList->begin(); i != moveList->end(); ++i)
	{
		if (bestMove->getScore() < (*i)->getScore())
		{
			bestMove = *i;
		}
	}
	
	return bestMove;
}
	
std::list<Move*>* Player::possibleMoves()
{
    std::list<Move*>* possibleMoves = new std::list<Move*>();
    Move* iteratorMove = new Move(0, 0);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            iteratorMove->setX(i);
            iteratorMove->setY(j);
            if (masterBoard->checkMove(iteratorMove, mySide))
            {
                Move* potentialMove = new Move(i, j);
                possibleMoves->push_back(potentialMove);
            }
        }
    }
    delete iteratorMove;
    return possibleMoves;
}

void Player::heuristic(Move* move, Side side)
{
	Board* tempBoard = masterBoard->copy();	//Copies board and applies move
	tempBoard->doMove(move,side);
	
	move->setScore(tempBoard->count(side));
	delete tempBoard;
}
