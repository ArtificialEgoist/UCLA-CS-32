#include <vector>
#include "actor.h"
#include "StudentWorld.h"
#include <cstdlib>
#include <ctime>

#include <iostream> //for testing purposes (cerr)

using namespace std;

StudentWorld::StudentWorld()
	: m_counter(0)
{}

StudentWorld::~StudentWorld()
{
	for(int i=0; i<m_actor.size(); i++) //if any actors are left, delete them just in case
		delete m_actor.at(i);
	m_actor.clear(); //clear the vector
}

void StudentWorld::init()
{
	srand(static_cast<unsigned int>(time(0))); //seed the random operator with the time
	
	Player* playerShip = new Player(this); //create the player and put it into the vector
	m_actor.push_back(playerShip);

	int numMushrooms;
	if(testParamsProvided())
		numMushrooms = getTestParam(TEST_PARAM_STARTING_MUSHROOMS); //determine number of Mushrooms to start with
	else
		numMushrooms = 25; //25 is the default number of Mushrooms to start with

	for(int i=0; i<numMushrooms; i++)
	{
		int randx = randInt(0, GARDEN_WIDTH-1);
		int randy = randInt(1, GARDEN_HEIGHT-2);
		
		while(mushroomThere(randx, randy)) //prevent Mushrooms from being created at the same location
		{
			randx = randInt(0, GARDEN_WIDTH-1);
			randy = randInt(1, GARDEN_HEIGHT-2);
		}

		Mushroom* mush = new Mushroom(this, randx, randy); //create a Mushroom and put it into the vector
		//For testing purposes: mush->setID(IID_POISON_MUSHROOM);
		m_actor.push_back(mush);
	}
}

int StudentWorld::move()
{
	m_counter++;

	//Code to add new insects
	addInsects();

	//Make current actors do something if they're alive
	for(int i=0; i<m_actor.size(); i++)
		if(m_actor.at(i)->isStillAlive())
			m_actor.at(i)->doSomething();

	bool playerDied=false; //Assume the player is still alive

	//Code to remove recently-dead objects
	for(int i=0; i<m_actor.size(); i++)
		if(!m_actor.at(i)->isStillAlive())
		{
			if(m_actor.at(i)->getID()==IID_PLAYER) //playerShip is dead
				playerDied=true;
			
			delete m_actor.at(i);
			
			vector<GameObject*>::iterator iter=m_actor.begin(); //create an iterator for the pointer index to be erased
			for(int j=0; j<i; j++)
				iter++;
			m_actor.erase(iter);
			i--;

		}

	// GWSTATUS_PLAYER_DIED if player recently died
	if(playerDied)
		return GWSTATUS_PLAYER_DIED; //if player died earlier, return corresponding value

	return GWSTATUS_CONTINUE_GAME; //otherwise, return a value to continue the game
}

void StudentWorld::cleanUp()
{
	for(int i=0; i<m_actor.size(); i++)
		delete m_actor.at(i); //delete every actor pointer
	m_actor.clear(); //clear the vector
}

void StudentWorld::storeObject(GameObject* temp)
{
	m_actor.push_back(temp); //push the GameObject pointer into the vector
}

void StudentWorld::attackObjects(int x, int y)
{
	for(int i=0; i<m_actor.size(); i++)
	{
		int xx, yy;
		GameObject* temp = m_actor.at(i);

		temp->getLocation(xx, yy);
		if(xx==x && yy==y) //For every object, if it's coordinates matches those in parameter, increase score
		{
			if(temp->getID()==IID_MUSHROOM || temp->getID()==IID_POISON_MUSHROOM)
			{
				increaseScore(1);
				((Mushroom*)temp)->lowerHealth(); //the mushroom sets itself dead when its health drops below a threshold
			}
			else
			{
				if(temp->getID()==IID_SCORPION)
					increaseScore(25);
				if(temp->getID()==IID_SPIDER)
					increaseScore(20);
				if(temp->getID()==IID_FLEA)
					increaseScore(50);
				if(temp->getID()==IID_SCENTIPEDE_SEGMENT)
					increaseScore(10);

				temp->setDead();
			}
		}
	}
}

bool StudentWorld::mushroomThere(int x, int y)
{
	for(int i=0; i<m_actor.size(); i++)
	{
		int id = m_actor.at(i)->getID();
		
		if(id==IID_MUSHROOM || id==IID_POISON_MUSHROOM) //for each mushroom in the world
		{
			int tempx, tempy;
			m_actor.at(i)->getLocation(tempx, tempy); //get its coordinates
			if(x==tempx && y==tempy) //if there is a mushroom at that spot, return true
				return true;
		}
	}
	return false;
}

bool StudentWorld::poisonMushroomThere(int x, int y)
{
	for(int i=0; i<m_actor.size(); i++)
	{
		int id = m_actor.at(i)->getID();
		
		if(id==IID_POISON_MUSHROOM) //for each mushroom in the world
		{
			int tempx, tempy;
			m_actor.at(i)->getLocation(tempx, tempy); //get its coordinates
			if(x==tempx && y==tempy) //if there is a mushroom at that spot, return true
				return true;
		}
	}
	return false;
}

bool StudentWorld::insectThere(int x, int y)
{
	for(int i=0; i<m_actor.size(); i++)
	{
		int id = m_actor.at(i)->getID();
		
		if(id==IID_SCENTIPEDE_SEGMENT || id==IID_SPIDER || id==IID_FLEA || id==IID_SCORPION) //for each insect in the world
		{
			int tempx, tempy;
			m_actor.at(i)->getLocation(tempx, tempy); //get its coordinates
			if(x==tempx && y==tempy) //if there is an insect at that spot, return true
				return true;
		}
	}
	return false;
}

bool StudentWorld::playerThere(int x, int y)
{
	for(int i=0; i<m_actor.size(); i++)
	{
		int id = m_actor.at(i)->getID();
		
		if(id==IID_PLAYER) //if player
		{
			int tempx, tempy;
			m_actor.at(i)->getLocation(tempx, tempy); //get its coordinates
			if(x==tempx && y==tempy) //if there is the player at that spot, return true
				return true;
		}
	}
	return false;
}

bool StudentWorld::scentipedeSegmentThere(int x, int y)
{
	for(int i=0; i<m_actor.size(); i++)
	{
		int id = m_actor.at(i)->getID();
		
		if(id==IID_SCENTIPEDE_SEGMENT) //if scentipede segment
		{
			int tempx, tempy;
			m_actor.at(i)->getLocation(tempx, tempy); //get its coordinates
			if(x==tempx && y==tempy) //if there is another scentipede segment at that spot, return true
				return true;
		}
	}
	return false;
}

void StudentWorld::dropMushroom(int x, int y)
{
	Mushroom* mush = new Mushroom(this, x, y); //create a new Mushroom at x,y and put it into vector
	m_actor.push_back(mush);
}

void StudentWorld::removeMushroom(int x, int y)
{
	int xx, yy;
	for(int i=0; i<m_actor.size(); i++)
	{
		m_actor.at(i)->getLocation(xx, yy); //if any normal/poisonous mushroom is at x,y
		if(xx==x && yy==y && (m_actor.at(i)->getID()==IID_MUSHROOM || m_actor.at(i)->getID()==IID_POISON_MUSHROOM))
			m_actor.at(i)->setDead(); //set it to dead
	}
}

void StudentWorld::makeMushroomPoisonous(int x, int y)
{
	int xx, yy;
	for(int i=0; i<m_actor.size(); i++)
	{
		m_actor.at(i)->getLocation(xx, yy);
		if(xx==x && yy==y && m_actor.at(i)->getID()==IID_MUSHROOM) //if there is a mushroom at x,y
			m_actor.at(i)->setID(IID_POISON_MUSHROOM); //change it to a poisonous one
	}
}

void StudentWorld::removeSegment(int x, int y)
{
	int xx, yy;
	for(int i=0; i<m_actor.size(); i++)
	{
		m_actor.at(i)->getLocation(xx, yy);
		if(xx==x && yy==y && m_actor.at(i)->getID()==IID_SCENTIPEDE_SEGMENT) //remove any scentipede segments at x,y
			m_actor.at(i)->setDead();
	}
}

void StudentWorld::killPlayer(int x, int y)
{
	int xx, yy;
	for(int i=0; i<m_actor.size(); i++)
	{
		m_actor.at(i)->getLocation(xx, yy);
		if(xx==x && yy==y && m_actor.at(i)->getID()==IID_PLAYER) //if player is at x,y
			m_actor.at(i)->setDead(); //set the player to dead
	}
}

void StudentWorld::addInsects()
{
	/*
	int m_insectCounter=0; //record how many insects there are in the world

	for(int i=0; i<m_actor.size(); i++)
	{
		int id = m_actor.at(i)->getID();
		if(id==IID_SCENTIPEDE_SEGMENT || id==IID_SPIDER || id==IID_FLEA || id==IID_SCORPION) //for each insect in the world
			m_insectCounter++; //if any actors are insects, increment the counter
	}
	*/

	if(testParamsProvided() && m_counter>0)
	{
		if(getTestParam(TEST_PARAM_CENT_CHANCE_INDEX)!=0 && m_counter%getTestParam(TEST_PARAM_CENT_CHANCE_INDEX)==0)
			addScentipede(); //add scentipede to garden
		if(getTestParam(TEST_PARAM_SPIDER_CHANCE_INDEX)!=0 && m_counter%getTestParam(TEST_PARAM_SPIDER_CHANCE_INDEX)==0)
			addSpider(); //add spider
		if(getTestParam(TEST_PARAM_FLEA_CHANCE_INDEX)!=0 && m_counter%getTestParam(TEST_PARAM_FLEA_CHANCE_INDEX)==0)
			addFlea(); //add flea
		if(getTestParam(TEST_PARAM_SCORPION_CHANCE_INDEX)!=0 && m_counter%getTestParam(TEST_PARAM_SCORPION_CHANCE_INDEX)==0)
			addScorpion(); //add scorpion
	}
	else
	{
		int currentLevel=getPlayerScore()/500; //calculate the player's level based on points
		int numScentipede=0;
		int numFlea=0;
		int numSpider=0;
		int numScorpion=0;
		bool scentipedeTopRow=false; //assume there are no scentipedes on top row

		for(int i=0; i<m_actor.size(); i++)
		{
			int id = m_actor.at(i)->getID();
			
			switch(id) //increment the counter for each insect type
			{
			case IID_SCENTIPEDE_SEGMENT:
				numScentipede++;
				
				int x, y;
				m_actor.at(i)->getLocation(x, y);
				if(y==GARDEN_HEIGHT-1)
					scentipedeTopRow=true; //if it is on the top row, update the boolean
				break;
			case IID_SPIDER:
				numSpider++;
				break;
			case IID_FLEA:
				numFlea++;
				break;
			case IID_SCORPION:
				numScorpion++;
				break;
			}
		}

		if(!(numScentipede>=min(20, 3*(currentLevel+1)) || scentipedeTopRow))
		{
			if(numScentipede==0)
			{
				if(randInt(1, 20)==1)
				{
					addScentipede(); //add scentipede
				}
			}
			else if(randInt(1, max(300-30*currentLevel, 50))==1)
			{
				addScentipede(); //add scentipede
			}
		}
		
		if(numFlea+numSpider+numScorpion < currentLevel/2+1)
		{
			if(randInt(1, max(300-currentLevel*30, 100))==1)
			{
				addFlea(); //add flea
			}
			else if(randInt(1, max(350-currentLevel*30, 100))==1)
			{
				addSpider(); //add spider
			}
			else if(randInt(1, max(400-currentLevel*30, 100))==1)
			{
				addScorpion(); //add scorpion
			}
		}
	}
}

void StudentWorld::addScentipede()
{
	int numToAdd = randInt(6, 12); //randomly determine how many segments (6-12) to add

	for(int i=numToAdd-1; i>=0; i--)
	{
		Scentipede* s = new Scentipede(this, i, GARDEN_HEIGHT-1); //for each segment place it in the right spot, head first
		m_actor.push_back(s);
	}
}

void StudentWorld::addFlea()
{
	int x = randInt(0, GARDEN_WIDTH-1);
	int y = GARDEN_HEIGHT-1;
	Flea* f = new Flea(this, x, y); //add a flea where it has any x coordinate at the very top
	m_actor.push_back(f);

}

void StudentWorld::addScorpion()
{
	int x = 0;
	int y = randInt(4, GARDEN_HEIGHT-1);
	Scorpion* s = new Scorpion(this, x, y); //add a scorpion where it has any y coordinate above 3 at the very left
	m_actor.push_back(s);
}

void StudentWorld::addSpider()
{
	int x = 0;
	if(randInt(1,2)==2)
		int x = GARDEN_WIDTH-1;
	//int y = randInt(4, GARDEN_HEIGHT-1);
	int y = randInt(5, GARDEN_HEIGHT-1); //changed due to new specs
	
	Spider* s = new Spider(this, x, y); //add a spider starting either on the left or right side, with y coordinate above 3
	m_actor.push_back(s);
}

int StudentWorld::randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest); //organize the numbers in times of size
    return lowest + (rand() % (highest - lowest + 1)); //return the corresponding random integer
}