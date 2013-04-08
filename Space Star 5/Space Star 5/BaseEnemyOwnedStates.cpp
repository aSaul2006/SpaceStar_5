#include "BaseEnemyOwnedStates.h"
#include "State, Message, and Entity Systems\State.h"
#include "State, Message, and Entity Systems\Telegram.h"
#include "State, Message, and Entity Systems\MessageSystem.h"
#include "State, Message, and Entity Systems\Timer.h"


//Enemy State Movement Behaviors
//attackPlayer1
attackPlayer1* attackPlayer1::Instance()
{
	static attackPlayer1 instance;
	return &instance;
}

void attackPlayer1::Enter(baseEnemyShip* pEnemyShip)
{
	//behavior for player attacking state1
	//for now we will define some basic attributes for this attack state

	pEnemyShip->setSpeed(10.0);
	pEnemyShip->setFireRate(2.0);
}

bool attackPlayer1::Execute(baseEnemyShip* pEnemyShip)
{
	//Execute this state
	return true;
}

void attackPlayer1::Exit(baseEnemyShip* pEnemyShip)
{

}

bool attackPlayer1::OnMessage(baseEnemyShip* agent, const Message& msg)
{
	return true;
}

//attackPlayer2
attackPlayer2* attackPlayer2::Instance()
{
	static attackPlayer2 instance;
	return &instance;
}

void attackPlayer2::Enter(baseEnemyShip* pEnemyShip)
{
	//behavior for player attacking state1
	
}

bool attackPlayer2::Execute(baseEnemyShip* pEnemyShip)
{
	//Execute this state
	return true;
}

void attackPlayer2::Exit(baseEnemyShip* pEnemyShip)
{
	//exit the state
}

bool attackPlayer2::OnMessage(baseEnemyShip* agent, const Message& msg)
{
	return true;
}

//attackPlayer3
attackPlayer3* attackPlayer3::Instance()
{
	static attackPlayer3 instance;
	return &instance;
}

void attackPlayer3::Enter(baseEnemyShip* pEnemyShip)
{
	//behavior for player attacking state1
	
}

bool attackPlayer3::Execute(baseEnemyShip* pEnemyShip)
{
	//Execute this state
	return true;
}

void attackPlayer3::Exit(baseEnemyShip* pEnemyShip)
{

}

bool attackPlayer3::OnMessage(baseEnemyShip* agent, const Message& msg)
{
	return true;
}

//avoidPlayer1
avoidPlayer1* avoidPlayer1::Instance()
{
	static avoidPlayer1 instance;
	return &instance;
}

void avoidPlayer1::Enter(baseEnemyShip* pEnemyShip)
{
	//behavior for player avoiding state1
	
}

bool avoidPlayer1::Execute(baseEnemyShip* pEnemyShip)
{
	//Execute this state
	return true;
}

void avoidPlayer1::Exit(baseEnemyShip* pEnemyShip)
{

}

bool avoidPlayer1::OnMessage(baseEnemyShip* agent, const Message& msg)
{
	return true;
}