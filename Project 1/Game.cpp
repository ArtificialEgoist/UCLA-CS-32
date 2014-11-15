#include "Game.h"
#include "Arena.h"
#include "globals.h"
#include "History.h"
#include "Player.h"

#include <cassert>
#include <iostream>
#include <cstdlib>
using namespace std;

class Player;


Game::Game(int rows, int cols, int nRobots)
{
    if (nRobots < 0  ||  nRobots > MAXROBOTS)
    {
        cout << "***** Game created with invalid number of robots:  "
             << nRobots << endl;
        exit(1);
    }

    int nEmpty = rows * cols - nRobots - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
             << cols << " arena, which is too small too hold a player and "
             << nRobots << " robots!" << endl;
        exit(1);
    }

      // Create arena
    m_arena = new Arena(rows, cols);

      // Add some walls in WALL_DENSITY of the empty spots
    assert(WALL_DENSITY >= 0  &&  WALL_DENSITY <= 1);
    int nWalls = static_cast<int>(WALL_DENSITY * nEmpty);
    while (nWalls > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (m_arena->getCellStatus(r, c) == WALL)
            continue;
        m_arena->setCellStatus(r, c, WALL);
        nWalls--;
    }

      // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with robots
    while (nRobots > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (m_arena->getCellStatus(r,c) != EMPTY  ||  (r == rPlayer && c == cPlayer))
            continue;
        m_arena->addRobot(r, c, randInt(1, MAXCHANNELS));
        nRobots--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x/h or nothing): ";
        string playerMove;
        getline(cin, playerMove);

        if (playerMove == "h")
        {
            //cout << "h command not yet implemented." << endl;

			m_arena->history().display();

            cout << "Press enter to continue.";
            cin.ignore(100000, '\n');
            m_arena->display("");
            continue;
        }

        Player* player = m_arena->player();
        int dir;

		string playerMovement;

        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
				return player->move(dir);
            else
				return player->stand();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
				return player->stand();
            else if (charToDir(playerMove[0], dir))
				return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x/h." << endl;
    }
}

string Game::takeRobotsTurn()
{
    for (;;)
    {
        cout << "Broadcast (e.g., 2n): ";
        string broadcast;
        getline(cin, broadcast);
        if (broadcast.size() != 2)
            cout << "Broadcast must be channel followed by direction." << endl;
        else if (broadcast[0] < '1'  ||  broadcast[0] > '0'+MAXCHANNELS)
            cout << "Channel must be a valid digit." << endl;
        else
        {
            int dir;
            if (charToDir(broadcast[1], dir))
                return m_arena->moveRobots(broadcast[0]-'0', dir);
            else
                cout << "Direction must be n, e, s, or w." << endl;
        }
    }
}


  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should stand; otherwise, bestDir is
  // set to the recommended direction to move.
bool Game::recommendMove(const Arena& a, int r, int c, int& bestDir)
{
      // How dangerous is it to stand?
    int standDanger = computeDanger(a, r, c);

      // if it's not safe, see if moving is safer
    if (standDanger > 0)
    {
        int bestMoveDanger = standDanger;
        int bestMoveDir = NORTH;  // arbitrary initialization

          // check the four directions to see if any move is
          // better than standing, and if so, record the best
        for (int dir = 0; dir < NUMDIRS; dir++)
        {
            int rnew = r;
            int cnew = c;
            if (attemptMove(a, dir, rnew, cnew))
            {
                int danger = computeDanger(a, rnew, cnew);
                if (danger < bestMoveDanger)
                {
                    bestMoveDanger = danger;
                    bestMoveDir = dir;
                }
            }
        }

          // if moving is better than standing, recommend move
        if (bestMoveDanger < standDanger)
        {
            bestDir = bestMoveDir;
            return true;
        }
    }
    return false;  // recommend standing
}


int Game::computeDanger(const Arena& a, int r, int c)
{
      // Our measure of danger will be the number of robots that might move
      // to position r,c.  If a robot is at that position, it is fatal,
      // so a large value is returned.

    if (a.numberOfRobotsAt(r,c) > 0)
        return MAXROBOTS+1;

    int danger = 0;
    if (r > 1)
        danger += a.numberOfRobotsAt(r-1,c);
    if (r < a.rows())
        danger += a.numberOfRobotsAt(r+1,c);
    if (c > 1)
        danger += a.numberOfRobotsAt(r,c-1);
    if (c < a.cols())
        danger += a.numberOfRobotsAt(r,c+1);

    return danger;
}

void Game::play()
{
    m_arena->display("");
    while ( ! m_arena->player()->isDead()  &&  m_arena->robotCount() > 0)
    {
        string msg = takePlayerTurn();
        m_arena->display(msg);
        Player* player = m_arena->player();
        if (player->isDead())
            break;
        msg = takeRobotsTurn();
        m_arena->display(msg);
    }
    if (m_arena->player()->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

