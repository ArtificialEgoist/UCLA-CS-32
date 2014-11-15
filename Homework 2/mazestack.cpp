#include <cassert>
#include <iostream>
#include <stack>

using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
	stack<Coord> cStack;
	cStack.push(Coord(sr, sc)); //Push the starting coordinate (sr,sc) onto the coordinate stack and

	maze[sr][sc]='%'; //update maze[sr][sc] to indicate that the algorithm has encountered
		
	while(!cStack.empty()) //While the stack is not empty,
	{
		Coord temp = cStack.top();
		cStack.pop(); //Pop the top coordinate off the stack. This gives you the current (r,c) location that your algorithm is exploring.

		int r = temp.r();
		int c = temp.c();

		cerr << "The coordinates popped were (" << r << ", " << c << ")" << endl;
		
		if(r==er && c==ec) //If the current (r,c) coordinate is equal to the ending coordinate, then we've solved the maze so return true! 
			return true;

		if(maze[r-1][c]=='.') //If you can move NORTH and haven't encountered that cell yet, then push the coordinate (r-1,c) onto the stack and update maze[r-1][c] to indicate the algorithm has encountered it.
		{
			cStack.push(Coord(r-1, c));
			maze[r-1][c]='%';
		}

		if(maze[r][c+1]=='.') //If you can move EAST...
		{
			cStack.push(Coord(r, c+1));
			maze[r][c+1]='%';
		}

		if(maze[r+1][c]=='.') //If you can move SOUTH...
		{
			cStack.push(Coord(r+1, c));
			maze[r+1][c]='%';
		}

		if(maze[r][c-1]=='.') //If you can move WEST...
		{
			cStack.push(Coord(r, c-1));
			maze[r][c-1]='%';
		}
	}
	return false; //There was no solution, so return false
}
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise



int main()
{
	char maze[10][10] = {
	    { 'X','X','X','X','X','X','X','X','X','X'},
	    { 'X','.','.','.','.','X','.','.','.','X'},
	    { 'X','.','X','X','.','X','X','.','.','X'},
	    { 'X','.','X','.','.','.','X','.','.','X'},
	    { 'X','X','X','X','X','.','X','.','.','X'},
	    { 'X','.','X','.','.','.','X','.','.','X'},
	    { 'X','.','.','.','X','.','X','.','.','X'},
	    { 'X','X','X','X','X','.','X','X','X','X'},
	    { 'X','.','.','.','.','.','.','.','.','X'},
	    { 'X','X','X','X','X','X','X','X','X','X'}
	};
	
	if (pathExists(maze,1,1,8,8))
	    cout << "Solvable!" << endl;
	else
	    cout << "Out of luck!" << endl;
}