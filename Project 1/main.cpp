
#include "Game.h"
#include "Arena.h"
#include "History.h"
#include "Player.h"
#include "Robot.h"
#include "globals.h"

#include <cstdlib>
#include <ctime>
using namespace std;

/*
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <cassert>
using namespace std;
*/



int main()
{
      // Initialize the random number generator
    srand(static_cast<unsigned int>(time(0)));

      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 50);

      // Play the game
    g.play();
}










/*

#include "Arena.h"
#include "Player.h"
#include "History.h"
#include "globals.h"

int main()
{
    Arena a(3, 3);
    a.addPlayer(3, 3);
	a.player()->stand();
	a.player()->move(NORTH);
	a.player()->stand();
	a.player()->stand();
	a.player()->move(WEST);
	a.player()->move(WEST);
	a.player()->move(NORTH);
	a.player()->move(NORTH);
	a.player()->move(WEST);
	a.player()->stand();
	a.player()->move(EAST);
	
	for(int i=0; i<25; i++)
		a.player()->move(EAST);
	
	a.history().display();
}
*/