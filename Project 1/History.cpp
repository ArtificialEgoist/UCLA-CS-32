#include "History.h"
#include "globals.h"

#include <iostream>
#include <cstdlib>
using namespace std;

/*
class History
{
	public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
};
*/

History::History(int nRows, int nCols)
	: m_nRows(nRows), m_nCols(nCols)
{
	int r, c;
	for (r = 1; r <= m_nRows; r++)
		for (c = 1; c <= m_nCols; c++)
	        m_historyGrid[r-1][c-1] = 0;
}

bool History::record(int r, int c)
{
	if(r>m_nRows || c>m_nCols) //if out of bounds, return false
		return false;

	m_historyGrid[r-1][c-1]+=1; //otherwise, increment the number of times player has been at that spot

	return true;
}

void History::display() const
{
    char displayHistory[MAXROWS][MAXCOLS];
    int r, c;

	for (r = 1; r <= m_nRows; r++)
        for (c = 1; c <= m_nCols; c++)
		{
			int numTimes = m_historyGrid[r-1][c-1];

			if(numTimes==0)
				displayHistory[r-1][c-1]='.';
			else if(numTimes>26)
				displayHistory[r-1][c-1]='Z';
			else
				displayHistory[r-1][c-1]=numTimes+64;
		}

	// Draw the grid
    clearScreen();

    for (r = 1; r <= m_nRows; r++)
    {
        for (c = 1; c <= m_nCols; c++)
            cout << displayHistory[r-1][c-1];
        cout << endl;
    }
    cout << endl;
}

