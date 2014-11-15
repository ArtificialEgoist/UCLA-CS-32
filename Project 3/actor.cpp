#include "actor.h"
#include "StudentWorld.h"
#include "GameController.h"

#include <iostream> //for testing purposes (cerr)

GameObject::GameObject(int id, StudentWorld* world)
	: GraphObject(id), myWorld(world), m_alive(true)
{
	displayMe(true);
}

GameObject::GameObject(int id, StudentWorld* world, int x, int y)
	: GraphObject(id), myWorld(world), m_alive(true)
{
	setInitialLocation(x, y); //set the object's position
	displayMe(true);
}

GameObject::~GameObject()
{
	displayMe(false);
}

bool GameObject::isStillAlive()
{
	return m_alive; //return whether the object is still alive
}

void GameObject::setDead()
{
	m_alive=false; //set the object to dead
}

StudentWorld* GameObject::getWorld() const
{
	return myWorld;
}

int GameObject::randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest); //organize the numbers in times of size
    return lowest + (rand() % (highest - lowest + 1)); //return the corresponding random integer
}

//////////////////////////////////////////////////

Player::Player(StudentWorld* world)
	: GameObject(IID_PLAYER, world, 15, 0)
{
	canShoot=true; //the player can shoot during the first tick (then it alternates)
}

Player::~Player()
{}

void Player::doSomething()
{
	int dir;
	StudentWorld *sw = getWorld();

	if(sw->getKey(dir)) //if there is an input
	{
		switch(dir)
		{
		case KEY_PRESS_SPACE:
			if(canShoot) //if the player can shoot a water droplet during this tick
			{
				int x, y;
				getLocation(x, y);

				//Determine the water droplets' positions first
				int dropletX = x;
				int dropletY = y+1;

				//while(dropletX>=0 && dropletX<GARDEN_WIDTH && dropletY>=0 && dropletY<GARDEN_HEIGHT)
				while(dropletX<=GARDEN_WIDTH-1 && dropletY<=GARDEN_HEIGHT-1)
				{
					if(getWorld()->insectThere(dropletX, dropletY) || getWorld()->mushroomThere(dropletX, dropletY))
					{
						getWorld()->attackObjects(dropletX, dropletY); //attack that object and give player points
						break; //no more droplets are to be shot!
					}
					else
					{
						WaterDroplet* w = new WaterDroplet(getWorld(), dropletX, dropletY); //Display a new water droplet
						getWorld()->storeObject(w);
						dropletY++; //Update the position of the next possible water droplet
					}
				}

				canShoot=false; //if a water droplet was shot this tick, make it so that one cannot be shot next tick
			}
			else
				canShoot=true; //if this round, no water droplet was shot, then one can be shot next tick
			break;
		default:
			movePlayer(dir); //if a key is pressed, move in that direction
			canShoot=true; //this counts as another tick, so next tick, another water droplet can be shot
			break;
		}
	}
	else
	{
		canShoot=true; //if canShoot was false earlier, no WaterDroplet was shot. Next round, the player can shoot again
	}

	int x, y;
	getLocation(x, y);
	if(getWorld()->insectThere(x, y)) //if the player is at or moves onto an insect, kill the player
		setDead();
	

}

void Player::movePlayer(int dir)
{
	int curx, cury;
	getLocation(curx, cury); //get the current location

	if(dir==KEY_PRESS_LEFT_ARROW && curx!=0) //as long as the new p osition is within bounds
	{
		if(getWorld()->insectThere(curx-1, cury)) //if there's an insect there, kill the player
			setDead();

		if(!getWorld()->mushroomThere(curx-1, cury)) //if there's no mushroom there, then the player can move there
			moveTo(curx-1, cury);
	}
	else if(dir==KEY_PRESS_RIGHT_ARROW && curx!=GARDEN_WIDTH-1)
	{
		if(getWorld()->insectThere(curx+1, cury))
			setDead();

		if(!getWorld()->mushroomThere(curx+1, cury))
			moveTo(curx+1, cury);
	}
	else if(dir==KEY_PRESS_UP_ARROW && cury!=3)
	{
		if(getWorld()->insectThere(curx, cury+1))
			setDead();

		if(!getWorld()->mushroomThere(curx, cury+1))
			moveTo(curx, cury+1);
	}
	else if(dir==KEY_PRESS_DOWN_ARROW && cury!=0)
	{
		if(getWorld()->insectThere(curx, cury-1))
			setDead();

		if(!getWorld()->mushroomThere(curx, cury-1))
			moveTo(curx, cury-1);
	}
}

//////////////////////////////////////////////////

Mushroom::Mushroom(StudentWorld* world, int x, int y)
	: GameObject(IID_MUSHROOM, world, x, y)
{
	 m_health=4; //mushrooms have 4 lives
}

Mushroom::~Mushroom()
{}

void Mushroom::doSomething()
{
	if(m_health<=0) //check if it's out of health
		setDead(); //if so, set it dead
}

int Mushroom::getHealth() const
{
	return m_health; //return the health
}

void Mushroom::lowerHealth()
{
	m_health--; //decrement health
	setBrightness(getBrightness()-0.25); //decrement brightness accordingly
}

//////////////////////////////////////////////////

WaterDroplet::WaterDroplet(StudentWorld* world, int x, int y)
	: GameObject(IID_WATER_DROPLET, world, x, y)
{
	m_ticks=2; //water droplets go away after two ticks
}

WaterDroplet::~WaterDroplet()
{}

void WaterDroplet::doSomething()
{
	m_ticks--; //every tick, the water droplet loses a tick count
	if(m_ticks<=0) //if it runs out, set it to dead
		setDead();
}

//////////////////////////////////////////////////

Insect::Insect(int id, StudentWorld* world, int x, int y)
	: GameObject(id, world, x, y)
{}

Insect::~Insect()
{}

//////////////////////////////////////////////////

Scentipede::Scentipede(StudentWorld* world, int x, int y)
	: Insect(IID_SCENTIPEDE_SEGMENT, world, x, y)
{
	movingDown=true;
	movingRight=true;
	isPoisoned=false; //a scentipede starts out unpoisoned, moving down and right
}

Scentipede::~Scentipede()
{}

void Scentipede::doSomething()
{
	int x, y;
	getLocation(x, y); //get current location
	
	if(isPoisoned && y==0)
		isPoisoned=false; //if it's at the bottom, it's no longer poisoned
	else if(isPoisoned) //if it's not yet at the bottom
	{
		if(getWorld()->mushroomThere(x, y-1))
			getWorld()->removeMushroom(x, y-1); //kill any mushroom it lands on
		
		getWorld()->killPlayer(x, y-1); //kill the player if it's there

		if(getWorld()->scentipedeSegmentThere(x, y-1))
			getWorld()->removeSegment(x, y-1); //kill any scentipede it lands on
	
		moveTo(x, y-1); //move down again

		if(isPoisoned)
			return; //any other insect is unaffected; since it's still poisoned, do nothing else
	}

	int newx, newy;
	getLocation(newx, newy); //get current location

	if(movingRight) //update the location without moving yet
		newx++;
	else
		newx--;

	if(newx<0)
		movingRight=true; //if it reaches left bounds, change diretion to right
	else if(newx==GARDEN_WIDTH)
	{
		movingRight=false; //if it reaches right bounds, change direction to left
	}
	else
	{
		if(!getWorld()->mushroomThere(newx, newy) && !getWorld()->playerThere(newx, newy) && !getWorld()->scentipedeSegmentThere(newx, newy))
		{
			moveTo(newx, newy); //if there's no mushroom, player, or scentipede, move to that position and return
			return;
		}
		else if(getWorld()->playerThere(newx, newy))
		{
			getWorld()->killPlayer(newx, newy); //if player is there, kill it
			return;
		}
		else if(getWorld()->poisonMushroomThere(newx, newy))
		{
			isPoisoned=true; //if a poisoned mushroom is there, poison the scentipede and change its direction to down
			movingDown=true;
		}
		//if there is a regular mushroom or another scentipede segment, do not move, but continue with the rest
	}

	if(movingDown) //update the vertical location
		newy--;
	else
		newy++;

	if(newy<0)
	{
		movingDown=false; //if it reaches the bottom bounds, change its direction to up
		newy=1;
	}
	else if(newy==GARDEN_HEIGHT)
	{
		movingDown=true; //if it reaches the top bounds, change its direction to down
		newy=GARDEN_HEIGHT-2;
	}

	if(!getWorld()->mushroomThere(newx, newy) && !getWorld()->playerThere(newx, newy) && !getWorld()->scentipedeSegmentThere(newx, newy))
	{
		moveTo(newx, newy); //if there's no mushroom, player, or scentipede, move to that position and return
		return;
	}
	else if(getWorld()->poisonMushroomThere(newx, newy))
	{
		isPoisoned=true; //set that mushroom to poisoned state again
		movingDown=true;
		newy--;
	}

	if(getWorld()->mushroomThere(newx, newy)) //remove the mushroom, player, or scentipede at its new spot
		getWorld()->removeMushroom(newx, newy);
	if(getWorld()->playerThere(newx, newy))
		getWorld()->killPlayer(newx, newy);
	if(getWorld()->scentipedeSegmentThere(newx, newy))
		getWorld()->removeSegment(newx, newy);

	moveTo(newx, newy); //move to that spot
}

void Scentipede::setDead()
{
	int x, y;
	getLocation(x, y);

	GameObject::setDead(); //set the scentipede to dead

	if(randInt(1, 3)==1)
		getWorld()->dropMushroom(x, y); //33% chance of spawning a mushroom where it died
}

//////////////////////////////////////////////////

Flea::Flea(StudentWorld* world, int x, int y)
	: Insect(IID_FLEA, world, x, y)
{}

Flea::~Flea()
{}

void Flea::doSomething()
{
	int x, y;
	getLocation(x, y); //get its current location

	if(!getWorld()->mushroomThere(x, y) && y>0 && y<GARDEN_HEIGHT-1) //if there's no mushroom there and it's within height bounds
		if(randInt(1, 4)==1)
			getWorld()->dropMushroom(x, y); //there's a 25% chance to drop a regular mushroom

	y--; //make the flea move down
	if(y<0) //if it leaves bottom bounds, set it to dead
		setDead();
	else
		moveTo(x, y); //otherwise, move to that new spot

	getWorld()->killPlayer(x, y);
}

//////////////////////////////////////////////////

Spider::Spider(StudentWorld* world, int x, int y)
	: Insect(IID_SPIDER, world, x, y)
{
	restNow=false; //spider can move for the first tick (but rests alternatingly)
	m_distance=randInt(1, 4); //give it a distance of 1 to 4 randomly
	movingDown=true; //starts out moving down
	movingRight=(x==0); //if the spider is all the way on the left, it is moving right
}

Spider::~Spider()
{}

void Spider::doSomething()
{
	if(restNow)
	{
		restNow=false; //rest every other tick
		return;
	}

	int x, y;
	getLocation(x, y); //get its current location

	if(m_distance==0) //if its distance is 0, flip its vertical direction and select a new distance
	{
		if(!movingDown) //if moving up
		{
			movingDown=true;	
			m_distance=randInt(1, y-1);
		}
		else //if moving down
		{
			movingDown=false;
			m_distance=randInt(1, GARDEN_HEIGHT-y-1);
		}
	}

	int newx, newy; //calculate the new diagonal location
	
	if(movingDown)
		newy=y-1;
	else
		newy=y+1;

	if(movingRight)
		newx=x+1;
	else
		newx=x-1;

	m_distance--; //decrement its vertical direction

	if(getWorld()->mushroomThere(newx, newy))
		getWorld()->removeMushroom(newx, newy); //if there is a mushroom, remove it

	if(newx<0 || newx>=GARDEN_WIDTH || newy<0 || newy>=GARDEN_HEIGHT) //if it moves out of bounds, set it to dead
		setDead();
	else
		moveTo(newx, newy); //move the spider

	getWorld()->killPlayer(newx, newy); //if it lands on player, kill it

	restNow=true; //if it didn't rest this tick, next tick it will rest
}

//////////////////////////////////////////////////

Scorpion::Scorpion(StudentWorld* world, int x, int y)
	: Insect(IID_SCORPION, world, x, y)
{}

Scorpion::~Scorpion()
{}

void Scorpion::doSomething()
{
	int x, y;
	getLocation(x, y); //get current location

	if(getWorld()->mushroomThere(x, y) && randInt(1, 3)==1) //if it's on a mushroom
		getWorld()->makeMushroomPoisonous(x, y); //there's a 33% chance of making it poisonous

	int newx=x+1; 

	if(newx<GARDEN_WIDTH-1) //move to the right if it's in bounds
		moveTo(newx, y);
	else
		setDead(); //otherwise, set it to dead

	getWorld()->killPlayer(newx, y); //if it lands on player, kill it
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////