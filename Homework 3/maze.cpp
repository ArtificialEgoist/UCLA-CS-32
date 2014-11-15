
/*
#include <cassert>
#include <iostream>

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
*/

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
	if(sr==er && sc==ec) //coordinates match, so a solution is found
		return true;

	maze[sr][sc]='%'; //update maze[sr][sc] to indicate that the algorithm has encountered

	if(maze[sr-1][sc]=='.') //If you can move NORTH and haven't encountered that cell yet, then try visiting it
	{
		if(pathExists(maze, sr-1, sc, er, ec))
			return true;
	}

	if(maze[sr][sc+1]=='.') //If you can move EAST...
	{
		if(pathExists(maze, sr, sc+1, er, ec))
			return true;
	}

	if(maze[sr+1][sc]=='.') //If you can move SOUTH...
	{
		if(pathExists(maze, sr+1, sc, er, ec))
			return true;
	}

	if(maze[sr][sc-1]=='.') //If you can move WEST...
	{
		if(pathExists(maze, sr, sc-1, er, ec))
			return true;
	}

	return false; //There was no solution, so return false
}
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise


/*
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
*/