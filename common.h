#ifndef __COMMON_H__
#define __COMMON_H__
#include <list>

enum Side { 
    WHITE, BLACK
};

class Move {
   
public:
    int x, y;
    Move(int x, int y) {
        this->x = x;
        this->y = y;
        this->score = 0;        
    }
    ~Move() {}

    int getX() { return x; }
    int getY() { return y; }
    int getScore() { return score; }

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
    void setScore(int score) { this->score = score; }

private:
    int score;
};

struct DecisionTreeNode
{
    DecisionTreeNode* parent;
    std::list<DecisionTreeNode*>* children;
    Move* currentMove;

    DecisionTreeNode(DecisionTreeNode* parentNode, Move* move, std::list<DecisionTreeNode*>* childList)
    {
        parent = parentNode;
        currentMove = move;
        children = childList;
    }
    DecisionTreeNode(DecisionTreeNode* parentNode, Move* move)
    {
        parent = parentNode;
        currentMove = move;
    }
    DecisionTreeNode* getParent()
    {
        return parent;
    }
    std::list<DecisionTreeNode*>* getChildren()
    {
        return children;
    }
    void addChildren(std::list<DecisionTreeNode*>* childList)
    {
        children = childList;
    }
    Move* getCurrentMove()
    {
        return currentMove;
    }
};

#endif
