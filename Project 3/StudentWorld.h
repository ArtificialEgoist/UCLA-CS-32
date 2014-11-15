#ifndef _StudentWorld_H_
#define _StudentWorld_H_

#include <vector>
#include "actor.h"
#include "GameWorld.h"

using namespace std;

class GameObject;

class StudentWorld: public GameWorld
{
public:
	StudentWorld::StudentWorld();
	~StudentWorld();
	virtual void init();
	virtual int move();
	virtual void cleanUp();

	//Checking for objects
	bool poisonMushroomThere(int x, int y);
	bool mushroomThere(int x, int y); //determine if any kind of mushroom is at x,y
	bool insectThere(int x, int y);
	bool playerThere(int x, int y);
	bool scentipedeSegmentThere(int x, int y);

	//Mushroom/object modifiers
	void storeObject(GameObject* temp); //add a GameObject pointer to the private vector
	void attackObjects(int x, int y); //let player attack all objects at x,y and get points
	void dropMushroom(int x, int y); //create a new mushroom at x,y
	void removeMushroom(int x, int y); //remove any kind of mushroom at x,y
	void makeMushroomPoisonous(int x, int y);
	void removeSegment(int x, int y);
	void killPlayer(int x, int y); //set the player to dead
	
	//Adding insects
	void addInsects();
	void addScentipede();
	void addFlea();
	void addScorpion();
	void addSpider();
private:
	vector<GameObject*> m_actor;
	int m_counter;
	int randInt(int lowest, int highest); //generate a random integer from lowest to highest, inclusively
};

#endif // #ifndef _StudentWorld_H_