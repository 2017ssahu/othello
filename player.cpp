#include "player.h"
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    temp = 0;
	this->mySide = side;
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
    
    if (opponentsMove != NULL)
    {	
		masterBoard->doMove(opponentsMove,opponentSide);
	}
	
    //Determine move 
    return miniMaxMove(2);
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

/**
 * Picks a move based on a simple heuristic
 * 
 */
Move* Player::simpleHeuristicMove()
{
	std::list<Move*>* moveList = possibleMoves(masterBoard,mySide);
	
	if (moveList == NULL)
	{
		return NULL;
	}
	
	for (std::list<Move*>::iterator i= moveList->begin(); i != moveList->end(); ++i)
	{
		(*i)->setScore(heuristic(*i,mySide,masterBoard));	//assigns a heuristic score to each move
	}
	
	Move* bestMove = *(moveList->begin()); //Set bestMove to the first move initially
	
	for (std::list<Move*>::iterator i= moveList->begin(); i != moveList->end(); ++i)
	{
		if (bestMove->getScore() < (*i)->getScore())
		{
			bestMove = *i;
		}
	}
	
	delete moveList;
	masterBoard->doMove(bestMove,mySide);	//Apply Move
	return bestMove;
}
	
/**
 * Returns of list of all possile moves by the side given as parameter
 */ 
std::list<Move*>* Player::possibleMoves(Board* tempBoard,Side side)
{
	if (!tempBoard->hasMoves(side))
	{
		return NULL;
	}
	
    std::list<Move*>* possibleMoves = new std::list<Move*>();
    Move* iteratorMove = new Move(0,0);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            iteratorMove->setX(i);
            iteratorMove->setY(j);
            if (tempBoard->checkMove(iteratorMove, side))
            {
                Move* potentialMove = new Move(i, j);
                possibleMoves->push_back(potentialMove);
            }
        }
    }
    
    delete iteratorMove;
    return possibleMoves;
}

/**
 * Returns the number of possible moves given a board and a side.
 */

int Player::mobilityFactor(Board* tempBoard,Side side)
{
    int mobilityFactor = 0;
    if (!tempBoard->hasMoves(side))
    {
        return  mobilityFactor;
    }
    
    Move* iteratorMove = new Move(0,0);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            iteratorMove->setX(i);
            iteratorMove->setY(j);
            if (tempBoard->checkMove(iteratorMove, side))
            {
                mobilityFactor++;
            }
            if (tempBoard->checkMove(iteratorMove, opponentSide))
            {
                mobilityFactor--;
            }
        }
    }
    
    delete iteratorMove;
    return mobilityFactor;
} 

/**
 * Returns the score of a a given board position with the move applied to it
 * Adds points for an increase in stones of the player side and for getting
 * corners or sides. Adds penalty for the spots near the corner
 * 
 */ 
int Player::heuristic(Move* move, Side side,Board* originalBoard)
{
	int score = 0;
	Board* tempBoard = originalBoard->copy();	//Copies board and applies move
	tempBoard->doMove(move,side);
	
	if (!testingMinimax)
	{
		score += (tempBoard->count(side) - originalBoard->count(side));
		if((move->getX() == 0 && move->getY() == 0) || (move->getX() == 0 && move->getY() == 7) || (move->getX() == 7 && move->getY() == 0) || (move->getX() == 7 && move->getY() == 7))
			score += 10;
		else if((move->getX() == 1 && move->getY() == 1) || (move->getX() == 1 && move->getY() == 6) || (move->getX() == 6 && move->getY() == 1) || (move->getX() == 6 && move->getY() == 6))
			score -= 10;
		else if((move->getX() == 1 && move->getY() == 0) || (move->getX() == 0 && move->getY() == 1) || (move->getX() == 1 && move->getY() == 7) || (move->getX() == 6 && move->getY() == 0) || (move->getX() == 0 && move->getY() == 6) || (move->getX() == 1 && move->getY() == 7) || (move->getX() == 6 && move->getY() == 7) || (move->getX() == 7 && move->getY() == 6))
			score -= 5;
		else if((move->getX() == 0) || (move->getX() == 7) || (move->getY() == 0) || (move->getY()) == 7)
			score += 5;
        else if((move->getX() == 1) || (move->getX() == 6) || (move->getY() == 1) || (move->getY()) == 6)
            score -= 3;
        //Adding in consideration to number of moves available
        score += mobilityFactor(tempBoard, side);

	}
	else
	{
		score += tempBoard->count(mySide) - tempBoard->count(opponentSide);
	}

	delete tempBoard;
	
	
    if(side == mySide)
	   return (score);
    else
        return (-score); 
}

/**
 * Finds the max heuristic score associated with a move contained within
 * a DecisionTreeNode element in the list passed as a parameter
 * 
 */ 
DecisionTreeNode* Player::findMin (std::list<DecisionTreeNode*>* list)
{
    DecisionTreeNode* min = list->front();
    for(std::list<DecisionTreeNode*>::iterator i = list->begin(); i != list->end(); i++)
    {
        if((*i)->getCurrentMove()->getScore() < min->getCurrentMove()->getScore())
        {
            min = *i;
        }
    }
    return min;
}

/**
 * Finds the min heuristic score associated with a move contained within
 * a DecisionTreeNode element in the list passed as a parameter
 * 
 */ 
DecisionTreeNode* Player::findMax (std::list<DecisionTreeNode*>* list)
{
    DecisionTreeNode* max = list->front();
    for(std::list<DecisionTreeNode*>::iterator i = list->begin(); i != list->end(); i++)
    {
        if((*i)->getCurrentMove()->getScore() > max->getCurrentMove()->getScore())
        {
            max = *i;
        }
    }
    return max;
}


/**
 * Uses a miniMax tree to determine the best course of action. Hard coded to explore
 * to a depth of two, but this will be changed in future iterations
 * 
 */ 
Move* Player::miniMaxMove(int depth)
{
	Board* tempBoard = masterBoard->copy();
	std::list<Move*>* masterMoveList = possibleMoves(tempBoard,mySide);
	DecisionTreeNode* parentNode = NULL;
	
	if (masterMoveList == NULL)	//Return NULL if there are no moves to be made
	{
		return NULL;
	}
	
	std::list<DecisionTreeNode*>* childrenList = new std::list<DecisionTreeNode*>();
	//converts from moveList to treeNode	
	for (std::list<Move*>::iterator i = masterMoveList->begin(); i != masterMoveList->end(); i++)
	{
		childrenList->push_back(new DecisionTreeNode(parentNode,*i));
	}
		
	for (std::list<DecisionTreeNode*>::iterator i = childrenList->begin(); i != childrenList->end(); i++)
	{
		parentNode = (*i);
		Move* cMove = parentNode->getCurrentMove();
		Board* secondLevel = tempBoard->copy();
		secondLevel->doMove(cMove,mySide);
		
		std::list<Move*>* secondMoveList = possibleMoves(secondLevel,opponentSide);
		
		if (secondMoveList != NULL)
		{
			std::list<DecisionTreeNode*>* secondChildrenList = new std::list<DecisionTreeNode*>();
			
			for (std::list<Move*>::iterator i = secondMoveList->begin(); i != secondMoveList->end(); i++)
			{
				secondChildrenList->push_back(new DecisionTreeNode(parentNode,*i));
			}
			
			parentNode->addChildren(secondChildrenList);
		}
		else
		{
			cMove->setScore(0);
		}
	}
	
	//Check values
	/*for(std::list<DecisionTreeNode*>::iterator i = childrenList->begin(); i != childrenList->end(); i++)
	{

		fprintf(stderr, "Black Moves: %d,%d\n",(*i)->getCurrentMove()->getX(),(*i)->getCurrentMove()->getY());
		std::list<DecisionTreeNode*>* children = (*i)->getChildren();
		
		if (children != NULL)
		{

			for (std::list<DecisionTreeNode*>::iterator j = children->begin(); j != children->end(); j++)
			{
				if (*j != NULL)
					fprintf(stderr, "\tWhite Moves: %d,%d\n",(*j)->getCurrentMove()->getX(),(*j)->getCurrentMove()->getY());
			}
			
			fprintf(stderr, "Finished White Moves\n");
		}
	}*/ 
	
	//Set the heuristic
	//Set bottom row of table first
	tempBoard = masterBoard->copy();
	
	//For every first level child
	for(std::list<DecisionTreeNode*>::iterator i = childrenList->begin(); i != childrenList->end(); i++)
	{
		tempBoard->doMove((*i)->getCurrentMove(),mySide);
		std::list<DecisionTreeNode*>* secondChildren = (*i)->getChildren();
		Board* tempSecondBoard = tempBoard->copy();
		
		if (secondChildren != NULL)
		{
			//For every second level child
			for (std::list<DecisionTreeNode*>::iterator j = secondChildren->begin(); j != secondChildren->end(); j++)
			{
					(*j)->getCurrentMove()->setScore(heuristic((*j)->getCurrentMove(),opponentSide,tempSecondBoard));
			}
			
			(*i)->getCurrentMove()->setScore(findMin(secondChildren)->getCurrentMove()->getScore());
		}
		tempBoard = masterBoard->copy();
	}
	
	Move* chosenMove = (findMax(childrenList))->getCurrentMove();
	masterBoard->doMove(chosenMove,mySide);
	return (chosenMove);
}

/**
 * Sets masterBoard to the board passed as parameter. Used to test minimax
 * 
 */ 
void Player::setBoard(Board* board)
{
	masterBoard = board;
}
