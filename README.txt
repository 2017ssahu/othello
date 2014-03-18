Avishek Dutta:
	-Wrote the doMove function
	-Wrote minimax functions 
	-Modified to use alpha beta pruning
	-Updated initial heuristic

Sarthak Sahu:
	-Wrote constructors
	-Wrote possibleMoves, findMax, findMin
	-Wrote initial heuristic and modified the second heuristic
	-Wrote DecisionTreeNode class

Initially we started with an algorithm that makes the first found possible move on the board. We have come a long way since then. After that, we implemented a minimax tree that worked iteratively in combination with our initial attempt at a heuristic. Now our algorithm implements a wide array of mechanisms to improve its playing ability. It uses a heuristic function that accounts for the importance of corners (coins in corners and those in positions near corners), stability (number of coins that are vulnerable to being flipped), mobility (number of moves available), parity (number of coins in our color vs. those of the opponents color), and edges (coins in edge positions). In addition, our algorithm implements alpha beta pruning which allows it to recursively search up to 6 levels down a minimax tree without running out of memory.