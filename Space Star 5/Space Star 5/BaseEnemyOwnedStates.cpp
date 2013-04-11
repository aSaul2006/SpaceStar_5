#include "BaseEnemyOwnedStates.h"
#include "State\State.h"
#include "State\Telegram.h"
#include "State\MessageDispatcher.h"
#include "Misc\CrudeTimer.h"


//Enemy State Movement Behaviors
//attackPlayer1
attackPlayer1* attackPlayer1::Instance()
{
	static attackPlayer1 instance;
	return &instance;
}

void attackPlayer1::Enter(Enemy* pEnemyShip)
{
	//behavior for player attacking state1
	//for now we will define some basic attributes for this attack state

	pEnemyShip->setSpeed(10.0);
	pEnemyShip->setFireRate(2.0);
}

void attackPlayer1::Execute(Enemy* pEnemyShip)
{
	//Execute this state
	
}

void attackPlayer1::Exit(Enemy* pEnemyShip)
{

}

bool attackPlayer1::OnMessage(Enemy* agent, const Telegram& msg)
{
	return true;
}

//attackPlayer2
attackPlayer2* attackPlayer2::Instance()
{
	static attackPlayer2 instance;
	return &instance;
}

void attackPlayer2::Enter(Enemy* pEnemyShip)
{
	//behavior for player attacking state1
	
}

void attackPlayer2::Execute(Enemy* pEnemyShip)
{
	//Execute this state
	
}

void attackPlayer2::Exit(Enemy* pEnemyShip)
{
	//exit the state
}

bool attackPlayer2::OnMessage(Enemy* agent, const Telegram& msg)
{
	return true;
}

//attackPlayer3
attackPlayer3* attackPlayer3::Instance()
{
	static attackPlayer3 instance;
	return &instance;
}

void attackPlayer3::Enter(Enemy* pEnemyShip)
{
	//behavior for player attacking state1
	
}

void attackPlayer3::Execute(Enemy* pEnemyShip)
{
	//Execute this state
	
}

void attackPlayer3::Exit(Enemy* pEnemyShip)
{

}

bool attackPlayer3::OnMessage(Enemy* agent, const Telegram& msg)
{
	return true;
}

//avoidPlayer1
avoidPlayer1* avoidPlayer1::Instance()
{
	static avoidPlayer1 instance;
	return &instance;
}

void avoidPlayer1::Enter(Enemy* pEnemyShip)
{
	//behavior for player avoiding state1
	
}

void avoidPlayer1::Execute(Enemy* pEnemyShip)
{
	//Execute this state
	
}

void avoidPlayer1::Exit(Enemy* pEnemyShip)
{

}

bool avoidPlayer1::OnMessage(Enemy* agent, const Telegram& msg)
{
	return true;
}