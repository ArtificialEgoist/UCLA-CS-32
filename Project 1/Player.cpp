#include "Player.h"
#include "Arena.h"
#include "globals.h"

#include <iostream>
#include <string>

#include <cstdlib>
using namespace std;

Player::Player(Arena* ap, int r, int c)
 : m_arena(ap), m_row(r), m_col(c), m_dead(false)
{
    if (ap == NULL)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::stand()
{
	m_arena->history().record(m_row, m_col); //if player doesn't move, record current position

    return "Player stands.";
}

string Player::move(int dir)
{
    if (attemptMove(*m_arena, dir, m_row, m_col))
    {
        if (m_arena->numberOfRobotsAt(m_row, m_col) > 0)
        {
            setDead();
            return "Player walked into a robot and died.";
        }
        string msg = "Player moved ";
        switch (dir)
        {
          case NORTH: msg += "north"; break;
          case EAST:  msg += "east";  break;
          case SOUTH: msg += "south"; break;
          case WEST:  msg += "west";  break;
        }
        msg += ".";

		m_arena->history().record(m_row, m_col); //if player moves, record new position

        return msg;
    }
    else
	{
		m_arena->history().record(m_row, m_col); //if player doesn't move, record current position

        return "Player couldn't move; player stands.";
	}
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}
