#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "GraphObj.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include "GameWorld.h"

class StudentWorld;

class GameObject : public GraphObject
{
public:
	GameObject(int id, StudentWorld* world);
	GameObject(int id, StudentWorld* world, int x, int y);
	virtual ~GameObject();
	virtual void doSomething() = 0; //virtual void, since varying actors have different implementations
	virtual bool isStillAlive();
	virtual void setDead();
	StudentWorld* getWorld() const;
	int randInt(int lowest, int highest); //generate a random integer from lowest to highest, inclusively
private:
	StudentWorld* myWorld;
	bool m_alive;
};

//////////////////////////////////////////////////

class Player : public GameObject
{
public:
	Player(StudentWorld* world);
	~Player();
	virtual void doSomething();
	void movePlayer(int dir);
private:
	bool canShoot;
};

//////////////////////////////////////////////////

class Mushroom : public GameObject
{
public:
	Mushroom(StudentWorld* world, int x, int y);
	~Mushroom();
	virtual void doSomething();
	int getHealth() const;
	void lowerHealth();
private:
	int m_health;
};

//////////////////////////////////////////////////

class WaterDroplet : public GameObject
{
public:
	WaterDroplet(StudentWorld* world, int x, int y);
	~WaterDroplet();
	virtual void doSomething();
private:
	int m_ticks;
};

//////////////////////////////////////////////////

class Insect : public GameObject
{
public:
	Insect(int id, StudentWorld* world, int x, int y);
	virtual ~Insect();
	virtual void doSomething() = 0;
private:
};

//////////////////////////////////////////////////

class Scentipede : public Insect
{
public:
	Scentipede(StudentWorld* world, int x, int y);
	~Scentipede();
	virtual void doSomething();
	virtual void setDead();
private:
	bool movingDown;
	bool movingRight;
	bool isPoisoned;
};

//////////////////////////////////////////////////

class Flea : public Insect
{
public:
	Flea(StudentWorld* world, int x, int y);
	~Flea();
	virtual void doSomething();
private:
};

//////////////////////////////////////////////////

class Spider : public Insect
{
public:
	Spider(StudentWorld* world, int x, int y);
	~Spider();
	virtual void doSomething();
private:
	bool restNow;
	int m_distance;
	bool movingDown;
	bool movingRight;
};

//////////////////////////////////////////////////

class Scorpion : public Insect
{
public:
	Scorpion(StudentWorld* world, int x, int y);
	~Scorpion();
	virtual void doSomething();
private:
};

//////////////////////////////////////////////////

#endif // #ifndef _ACTOR_H_
