#ifndef GAME_INCLUDED
#define GAME_INCLUDED

class Arena;

#include <string>

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nRobots);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    std::string takePlayerTurn();
    std::string takeRobotsTurn();
	bool recommendMove(const Arena& a, int r, int c, int& bestDir);
	int computeDanger(const Arena& a, int r, int c);
};

#endif	//GAME_INCLUDED