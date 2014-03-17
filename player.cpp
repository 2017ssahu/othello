#include "player.h"
#include "weights.h"
#define ndx(obj,i,j,n)      (obj[(i)+(j)*n])
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
	double alpha = -100.0;
	double beta = 100.0;
	
	std::list<DecisionTreeNode*>* childrenList = new std::list<DecisionTreeNode*>();
	std::list<Move*>* moveList = possibleMoves(masterBoard,mySide);
	DecisionTreeNode* parentNode = NULL;
	
	if (moveList->empty())
	{
		return NULL;
	}
	
	//converts from moveList to treeNode	
	for (std::list<Move*>::iterator i = moveList->begin(); i != moveList->end(); i++)
	{
		DecisionTreeNode* nextNode = new DecisionTreeNode(parentNode,*i);
		childrenList->push_back(nextNode);	
		Board* boardCopy = masterBoard->copy();
		boardCopy->doMove(*i,mySide);
		miniMaxMove(depth-1,boardCopy,opponentSide,nextNode,alpha,beta); //depth,board for move,side,parent
		//This should create tree with nextNode as its parent
	}
	
	Move* chosenMove = (findMax(childrenList))->getCurrentMove();
	masterBoard->doMove(chosenMove,mySide);
	//printTree(childrenList);
	
    delete moveList; //Freeing Memory
    delete childrenList; //Freeing Memory
	
    return (chosenMove);
}

//Recursive Minimax algorithm
double Player::miniMaxMove(int depth,Board* board, Side side, DecisionTreeNode* node, double alpha, double beta)
{
	Move* move = node->getCurrentMove(); //Retrieve move from node
	
	if (depth == 0)	//Base case, evaluate heuristic and return
	{
		double score = heuristic(flip(side),board); 
		move->setScore(score);
		return score;
	}
	else
	{
		std::list<Move*>* moveList = possibleMoves(board,side);	//populate possible moves
		
		if (moveList->empty()) 	//No more moves? Evaluate heuristic and return
		{
			double score = heuristic(flip(side),board); 
			move->setScore(score);
			delete moveList;
			return score;
		}
		else
		{
			std::list<DecisionTreeNode*>* childrenList = new std::list<DecisionTreeNode*>();
			//Setup up childrenList variable
			if (side == mySide)
			{
				for (std::list<Move*>::iterator i = moveList->begin(); i != moveList->end(); i++)
				{
					DecisionTreeNode* nextNode = new DecisionTreeNode(node,*i);
					childrenList->push_back(nextNode);	
					
					Board* boardCopy = board->copy();
					boardCopy->doMove(*i,side);
					alpha = max(alpha,miniMaxMove(depth-1,boardCopy,flip(side),nextNode,alpha,beta)); 
					//This should create tree with nextNode as its parent
					
					if (beta <= alpha) //Player has made mistake, do not consider further
					{
						break;
					}
				}
				
				node->addChildren(childrenList);	//Setup in case we wish to use the tree later
				move->setScore(alpha);
				delete moveList;
				return alpha;
			}
			else  		//Same as before except for minimizing player
			{
				for (std::list<Move*>::iterator i = moveList->begin(); i != moveList->end(); i++)
				{
					DecisionTreeNode* nextNode = new DecisionTreeNode(node,*i);
					childrenList->push_back(nextNode);	
					
					Board* boardCopy = board->copy();
					boardCopy->doMove(*i,side);
					beta = min(beta,miniMaxMove(depth-1,boardCopy,flip(side),nextNode,alpha,beta)); 
					//This should create tree with nextNode as its parent
					
					if (beta <= alpha) //Player has made mistake, do not need to consider
					{
						break;
					}
				}
				
				node->addChildren(childrenList);
				move->setScore(beta);
				delete moveList; //Free memory
				return beta;
			}

            //delete childrenList; //Freeing Memory
		}
	}
}
	
/**
 * Returns of list of all possile moves by the side given as parameter
 */ 
std::list<Move*>* Player::possibleMoves(Board* tempBoard,Side side)
{
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

double Player::heuristic(Side side, Board* nextBoard)
{
	int myCoins = nextBoard->count(mySide);
	int opponentCoins = nextBoard->count(opponentSide);
	
	std::list<Move*>* myMoveList = possibleMoves(nextBoard,mySide);
	std::list<Move*>* opponentMoveList = possibleMoves(nextBoard,opponentSide);
	int myMoves = myMoveList->size();
	int opponentMoves = opponentMoveList->size();
	delete myMoveList;
	delete opponentMoveList;
	
	int myStability = 0;
	int opponentStability = 0;
    int myCorners = 0;
    int opponentCorners = 0;
    int myInnerCorners = 0;
    int opponentInnerCorners = 0;
    int myAdjacentCorners = 0;
    int opponentAdjacentCorners = 0;
	
	for (int i = 0; i < 8; i++)	//Check who owns what pieces and where
	{
		for (int j = 0; j < 8; j++)
		{
			if (nextBoard->occupied(i,j))
			{
				if (nextBoard->getColor(i,j) == mySide)
				{
					myStability += ndx(weights,i,j,8);
                    if ((i == 0 && j == 0) || (i == 7 && j == 0) || (i == 0 && j == 7) || (i == 7 && j == 7))
                    {
                        myCorners++;
                    }
                    if ((i == 1 && j == 1) || (i == 6 && j == 1) || (i == 1 && j == 6) || (i == 6 && j == 6))
                    {
                        myInnerCorners++;
                    }
                    if ((i == 1 && j == 0) || (i == 0 && j == 1) || (i == 1 && j == 7) || (i == 0 && j == 6) || (i == 7 && j == 1) || (i == 6 && j == 0) || (i == 7 && j == 6) || (i == 6 && j == 7))
                    {
                        myAdjacentCorners++;
                    }
				}
				else
				{
					opponentStability += ndx(weights,i,j,8);
                    if ((i == 0 && j == 0) || (i == 7 && j == 0) || (i == 0 && j == 7) || (i == 7 && j == 7))
                    {
                        opponentCorners++;
                    }
                    if ((i == 1 && j == 1) || (i == 6 && j == 1) || (i == 1 && j == 6) || (i == 6 && j == 6))
                    {
                        opponentInnerCorners++;
                    }
                    if ((i == 1 && j == 0) || (i == 0 && j == 1) || (i == 1 && j == 7) || (i == 0 && j == 6) || (i == 7 && j == 1) || (i == 6 && j == 0) || (i == 7 && j == 6) || (i == 6 && j == 7))
                    {
                        opponentAdjacentCorners++;
                    }
				}
			}
		}
	}
	
	//Number of coins relative to opponent
	double coinParity = 100 * (myCoins - opponentCoins)/(myCoins + opponentCoins);
	
	//Mobility relative to opponent
	double mobility = 0;
	if ((myMoves + opponentMoves) != 0)
	{
		mobility = 100 * (myMoves - opponentMoves)/(myMoves + opponentMoves);
	}
	
	//Corners relative to opponent
    double corners = 25 * myCorners - 25 * opponentCorners;
    double innerCorners = -25 * myInnerCorners + 25 * opponentInnerCorners;
    double adjacentCorners = -12.5 * myAdjacentCorners + 12.5 * opponentAdjacentCorners;
	//Stability
	double stability = 0;
	if ((myStability + opponentStability) != 0)
	{
		stability = 100 * (myStability - opponentStability)/(myStability + opponentStability);
	}
	
	return ((coinParity + 2 * mobility + 2 * stability + 4 * corners + 3 * innerCorners + 2 * adjacentCorners)/14.0);
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

double Player::max(double one, double two)
{
	if (one > two)
	{
		return one;
	}
	else
	{
		return two;
	}
}

double Player::min(double one, double two)
{
	if (one < two)
	{
		return one;
	}
	else
	{
		return two;
	}
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
			fprintf(stderr, "White Moves: %d,%d\tScore: %f\n",node->getCurrentMove()->getX(),node->getCurrentMove()->getY(),node->getCurrentMove()->getScore());
			std::list<DecisionTreeNode*>* nextChildrenList = node->getChildren();
			
			if (nextChildrenList != NULL)
			{
				for (std::list<DecisionTreeNode*>::iterator j = nextChildrenList->begin(); j != nextChildrenList->end(); j++)
				{
					DecisionTreeNode* node1 = (*j);
					fprintf(stderr, "\tBlack Responses: %d,%d\tScore: %f\n",node1->getCurrentMove()->getX(),node1->getCurrentMove()->getY(),node1->getCurrentMove()->getScore());
					std::list<DecisionTreeNode*>* thirdChildrenList  = node1->getChildren();
				
					if (thirdChildrenList != NULL)
					{
						for (std::list<DecisionTreeNode*>::iterator k = thirdChildrenList->begin(); k != thirdChildrenList->end(); k++)
						{
							DecisionTreeNode* node2 = (*k);
							fprintf(stderr, "\t\tWhite Responses: %d,%d\tScore: %f\n",node2->getCurrentMove()->getX(),node2->getCurrentMove()->getY(),node2->getCurrentMove()->getScore());
							std::list<DecisionTreeNode*>* fourthChildrenList  = node2->getChildren();
				
							if (fourthChildrenList != NULL)
							{
								for (std::list<DecisionTreeNode*>::iterator l = fourthChildrenList->begin(); l != fourthChildrenList->end(); l++)
								{
									DecisionTreeNode* node3 = (*l);
									fprintf(stderr, "\t\t\tBlack Responses: %d,%d\tScore: %f\n",node3->getCurrentMove()->getX(),node3->getCurrentMove()->getY(),node3->getCurrentMove()->getScore());
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
