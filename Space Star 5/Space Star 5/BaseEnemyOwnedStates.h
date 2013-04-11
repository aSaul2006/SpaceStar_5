#pragma once



#include "EnemyShip.h"
#include "State\State.h"
#include "State\MessageDispatcher.h"


//define states for ships to use for example pursuePlayer, avoidTarget.
class attackPlayer1 : public State<Enemy>
{
private:
	attackPlayer1(){}
	//copy ctor and assignment 
	attackPlayer1(const attackPlayer1&);
	attackPlayer1 & operator=(const attackPlayer1&);
public:
	//will always be a singleton class
	static attackPlayer1* Instance();
	virtual void Enter(Enemy* baseEnemy);
	virtual void Execute(Enemy* baseEnemy);
	virtual void Exit(Enemy* baseEnemy);
	virtual bool OnMessage(Enemy* agent, const Telegram& msg);
};

class attackPlayer2 : public State<Enemy>
{
private:
	attackPlayer2(){}
	//copy ctor and assignment 
	attackPlayer2(const attackPlayer2&);
	attackPlayer2 & operator=(const attackPlayer2&);
public:
	//will always be a singleton class
	static attackPlayer2* Instance();
	virtual void Enter(Enemy* baseEnemy);
	virtual void Execute(Enemy* baseEnemy);
	virtual void Exit(Enemy* baseEnemy);
	virtual bool OnMessage(Enemy* agent, const Telegram& msg);
};

class attackPlayer3 : public State<Enemy>
{
private:
	attackPlayer3(){}
	//copy ctor and assignment 
	attackPlayer3(const attackPlayer3&);
	attackPlayer3 & operator=(const attackPlayer3&);
public:
	//will always be a singleton class
	static attackPlayer3* Instance();
	virtual void Enter(Enemy* baseEnemy);
	virtual void Execute(Enemy* baseEnemy);
	virtual void Exit(Enemy* baseEnemy);
	virtual bool OnMessage(Enemy* agent, const Telegram& msg);
};

class avoidPlayer1 : public State<Enemy>
{
private:
	avoidPlayer1(){}
	avoidPlayer1(const avoidPlayer1&);
	avoidPlayer1 & operator=(const avoidPlayer1&);
public:
	static avoidPlayer1* Instance();
	virtual void Enter(Enemy* baseEnemy);
	virtual void Execute(Enemy* baseEnemy);
	virtual void Exit(Enemy* baseEnemy);
	virtual bool OnMessage(Enemy* agent, const Telegram& msg);
};
//create ship classes that derive Enemy
