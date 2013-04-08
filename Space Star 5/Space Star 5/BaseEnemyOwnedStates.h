#pragma once
#ifndef _BASE_ENEMY_OWNED_STATES_H
#define _BASE_ENEMY_OWNED_STATES_H


#include "EnemyShip.h"
#include "State, Message, and Entity Systems\State.h"


//define states for ships to use for example pursuePlayer, avoidTarget.
class attackPlayer1 : public State<baseEnemyShip>
{
private:
	attackPlayer1(){}
	//copy ctor and assignment 
	attackPlayer1(const attackPlayer1&);
	attackPlayer1 & operator=(const attackPlayer1&);
public:
	//will always be a singleton class
	static attackPlayer1* Instance();
	void Enter(baseEnemyShip* baseEnemy);
	bool Execute(baseEnemyShip* baseEnemy);
	void Exit(baseEnemyShip* baseEnemy);
	bool OnMessage(baseEnemyShip* agent, const Message& msg);
};

class attackPlayer2 : public State<baseEnemyShip>
{
private:
	attackPlayer2(){}
	//copy ctor and assignment 
	attackPlayer2(const attackPlayer2&);
	attackPlayer2 & operator=(const attackPlayer2&);
public:
	//will always be a singleton class
	static attackPlayer2* Instance();
	void Enter(baseEnemyShip* baseEnemy);
	bool Execute(baseEnemyShip* baseEnemy);
	void Exit(baseEnemyShip* baseEnemy);
	bool OnMessage(baseEnemyShip* agent, const Message& msg);
};

class attackPlayer3 : public State<baseEnemyShip>
{
private:
	attackPlayer3(){}
	//copy ctor and assignment 
	attackPlayer3(const attackPlayer3&);
	attackPlayer3 & operator=(const attackPlayer3&);
public:
	//will always be a singleton class
	static attackPlayer3* Instance();
	void Enter(baseEnemyShip* baseEnemy);
	bool Execute(baseEnemyShip* baseEnemy);
	void Exit(baseEnemyShip* baseEnemy);
	bool OnMessage(baseEnemyShip* agent, const Message& msg);
};

class avoidPlayer1 : public State<baseEnemyShip>
{
private:
	avoidPlayer1(){}
	avoidPlayer1(const avoidPlayer1&);
	avoidPlayer1 & operator=(const avoidPlayer1&);
public:
	static avoidPlayer1* Instance();
	void Enter(baseEnemyShip* baseEnemy);
	bool Execute(baseEnemyShip* baseEnemy);
	void Exit(baseEnemyShip* baseEnemy);
	bool OnMessage(baseEnemyShip* agent, const Message& msg);
};
//create ship classes that derive baseEnemyShip

#endif