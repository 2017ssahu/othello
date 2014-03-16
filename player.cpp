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
    return miniMaxMove(4);
}

//This is the helper function that goes on to call the recursive miniMax
Move* Player::miniMaxMove(int depth)
{
	std::list<DecisionTreeNode*>* childrenList = new std::list<DecisionTreeNode*>();
	std::list<Move*>* moveList = possibleMoves(masterBoard,mySide);
	DecisionTreeNode* parentNode = NULL;
	
	if (moveList == NULL)
	{
		return NULL;
	}
	
	//converts from moveList to treeNode	
	for (std::list<Move*>::iterator i = moveList->begin(); i != moveList->end(); i++)
	{
		DecisionTreeNode* nextNode = new DecisionTreeNode(parentNode,*i);
		childrenList->push_back(nextNode);	
		Board* boardCopy = masterBoard->copy();
		miniMaxMove(depth-1,boardCopy,opponentSide,nextNode); //depth,board for move,side,parent
		//This should create tree with nextNode as its parent
	}
	
	Move* chosenMove = (findMax(childrenList))->getCurrentMove();
	masterBoard->doMove(chosenMove,mySide);
	//printTree(childrenList);
	return (chosenMove);
}

//Recursive Minimax algorithm
void Player::miniMaxMove(int depth,Board* board, Side side, DecisionTreeNode* node)
{
	Move* move = node->getCurrentMove();
	
	if (depth == 0)
	{
		move->setScore(heuristic(move,flip(side),board));
	}
	else
	{
		board->doMove(move,flip(side));
		std::list<Move*>* moveList = possibleMoves(board,side);
		
		if (moveList == NULL) //Perhaps modify possibleMoves and use .isEmpty() instead
		{
			if (side == mySide)
			{
				move->setScore(-1000);
			}
			else
			{
				move->setScore(1000);
			}
		}
		else
		{
			std::list<DecisionTreeNode*>* childrenList = new std::list<DecisionTreeNode*>();
			
			for (std::list<Move*>::iterator i = moveList->begin(); i != moveList->end(); i++)
			{
				DecisionTreeNode* nextNode = new DecisionTreeNode(node,*i);
				childrenList->push_back(nextNode);	
				Board* boardCopy = board->copy();
				miniMaxMove(depth-1,boardCopy,flip(side),nextNode); //depth,move,board for move,side,parent
				//This should create tree with nextNode as its parent
			}
			
			node->addChildren(childrenList);
			
			if (side == mySide)
			{
				move->setScore(findMax(childrenList)->getCurrentMove()->getScore());
			}
			else
			{
				move->setScore(findMin(childrenList)->getCurrentMove()->getScore());
			}
		}
		
	}
}

/**
 * Uses a miniMax tree to determine the best course of action. Hard coded to explore
 * to a depth of two, but this will be changed in future iterations
 * 
 */ 
Move* Player::miniMaxMove()
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
	//printTree(childrenList);
	return (chosenMove);
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
            if (tempBoard->checkMove(iteratorMove, flip(side)))
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
int Player::heuristic(Move* move, Side side, Board* originalBoard)
{
	int score = 0;
	Board* tempBoard = originalBoard->copy();	//Copies board and applies move
	tempBoard->doMove(move,side);
	
	if (!testingMinimax)
	{
		score += tempBoard->count(mySide) - tempBoard->count(opponentSide);
		//score += (tempBoard->count(side) - originalBoard->count(side));
		
		if (side == mySide)
		{
			if((move->getX() == 0 && move->getY() == 0) || (move->getX() == 0 && move->getY() == 7) || (move->getX() == 7 && move->getY() == 0) || (move->getX() == 7 && move->getY() == 7))
				score += 100;
			else if((move->getX() == 1 && move->getY() == 1) || (move->getX() == 1 && move->getY() == 6) || (move->getX() == 6 && move->getY() == 1) || (move->getX() == 6 && move->getY() == 6))
				score -= 100;
			else if((move->getX() == 1 && move->getY() == 0) || (move->getX() == 0 && move->getY() == 1) || (move->getX() == 1 && move->getY() == 7) || (move->getX() == 6 && move->getY() == 0) || (move->getX() == 0 && move->getY() == 6) || (move->getX() == 1 && move->getY() == 7) || (move->getX() == 6 && move->getY() == 7) || (move->getX() == 7 && move->getY() == 6))
				score -= 75;
			else if((move->getX() == 0) || (move->getX() == 7) || (move->getY() == 0) || (move->getY()) == 7)
				score += 25;
			else if((move->getX() == 1) || (move->getX() == 6) || (move->getY() == 1) || (move->getY()) == 6)
				score -= 20;
		}
		else //Double check these things
		{
			if((move->getX() == 0 && move->getY() == 0) || (move->getX() == 0 && move->getY() == 7) || (move->getX() == 7 && move->getY() == 0) || (move->getX() == 7 && move->getY() == 7))
				score -= 100;
			else if((move->getX() == 1 && move->getY() == 1) || (move->getX() == 1 && move->getY() == 6) || (move->getX() == 6 && move->getY() == 1) || (move->getX() == 6 && move->getY() == 6))
				score += 100;
			else if((move->getX() == 1 && move->getY() == 0) || (move->getX() == 0 && move->getY() == 1) || (move->getX() == 1 && move->getY() == 7) || (move->getX() == 6 && move->getY() == 0) || (move->getX() == 0 && move->getY() == 6) || (move->getX() == 1 && move->getY() == 7) || (move->getX() == 6 && move->getY() == 7) || (move->getX() == 7 && move->getY() == 6))
				score += 75;
			else if((move->getX() == 0) || (move->getX() == 7) || (move->getY() == 0) || (move->getY()) == 7)
				score -= 25;
			else if((move->getX() == 1) || (move->getX() == 6) || (move->getY() == 1) || (move->getY()) == 6)
				score += 20;
		}
        //Adding in consideration to number of moves available
        //fprintf(stderr, "Mobility Factor: %d\n", mobilityFactor(tempBoard, side));
        //score += 3 * mobilityFactor(tempBoard, side); 

	}
	else
	{
		score += tempBoard->count(mySide) - tempBoard->count(opponentSide);
	}

	delete tempBoard;
	
    /*if(side == mySide)
	   return (score);
    else
        return (-1 * score); */
    return score;
}

Side Player::flip(Side side)
{
	if (side == BLACK)
	{
		return WHITE;
	}
	else
	{
		return BLACK;
	}
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
 * Sets masterBoard to the board passed as parameter. Used to test minimax
 * 
 */ 
void Player::setBoard(Board* board)
{
	masterBoard = board;
}

//Prints the tree rooted by the nodes in the children list
//Hard coded to print out 4 levels
void Player::printTree(std::list<DecisionTreeNode*>* childrenList)
{
	if (childrenList != NULL)
	{
		for (std::list<DecisionTreeNode*>::iterator i = childrenList->begin(); i != childrenList->end(); i++)
		{
			DecisionTreeNode* node = (*i);
			fprintf(stderr, "White Moves: %d,%d\tScore: %d\n",node->getCurrentMove()->getX(),node->getCurrentMove()->getY(),node->getCurrentMove()->getScore());
			std::list<DecisionTreeNode*>* nextChildrenList = node->getChildren();
			
			if (nextChildrenList != NULL)
			{
				for (std::list<DecisionTreeNode*>::iterator j = nextChildrenList->begin(); j != nextChildrenList->end(); j++)
				{
					DecisionTreeNode* node1 = (*j);
					fprintf(stderr, "\tBlack Responses: %d,%d\tScore: %d\n",node1->getCurrentMove()->getX(),node1->getCurrentMove()->getY(),node1->getCurrentMove()->getScore());
					std::list<DecisionTreeNode*>* thirdChildrenList  = node1->getChildren();
				
					if (thirdChildrenList != NULL)
					{
						for (std::list<DecisionTreeNode*>::iterator k = thirdChildrenList->begin(); k != thirdChildrenList->end(); k++)
						{
							DecisionTreeNode* node2 = (*k);
							fprintf(stderr, "\t\tWhite Responses: %d,%d\tScore: %d\n",node2->getCurrentMove()->getX(),node2->getCurrentMove()->getY(),node2->getCurrentMove()->getScore());
							std::list<DecisionTreeNode*>* fourthChildrenList  = node2->getChildren();
				
							if (fourthChildrenList != NULL)
							{
								for (std::list<DecisionTreeNode*>::iterator l = fourthChildrenList->begin(); l != fourthChildrenList->end(); l++)
								{
									DecisionTreeNode* node3 = (*l);
									fprintf(stderr, "\t\t\tBlack Responses: %d,%d\tScore: %d\n",node3->getCurrentMove()->getX(),node3->getCurrentMove()->getY(),node3->getCurrentMove()->getScore());
								}
							}
							else
							{
								fprintf(stderr, "\t\t\tNo Black Responses\n");
							}
						}
					}
					else
					{
						fprintf(stderr, "\t\tNo White Responses\n");
					}
				}
			}
			else
			{
				fprintf(stderr, "\tNo Black Responses\n");
			}
		}
	} 
}
