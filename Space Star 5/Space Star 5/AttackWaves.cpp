#include "AttackWaves.h"


AttackWaves::~AttackWaves()
{

}

AttackWaves::AttackWaves()
{

}

ViperWave1::~ViperWave1()
{
}

ViperWave1::ViperWave1()
{
	enemyAmount = 0;
}

void ViperWave1::Shutdown()
{

}

void ViperWave1::AttackPattern(AttackType at)
{
	for(int i = 0;i < 5;i++)
	{
		pEnemiesInViperOne.push_front(new Enemy());
	}

	for(list<Enemy*>::const_iterator i = pEnemiesInViperOne.begin(), end = pEnemiesInViperOne.end(); i != end;)
	{
		float yPos = -4.0;
		(*i)->initializeEnemyShip();
		(*i)->SetEnemyAttrib(100,5.0f,2.0f,D3DXVECTOR3(12.0f,yPos,0.0f));
		yPos += 2.0;
	}
}

void ViperWave1::SetEnemyAmount(int amount)
{
	enemyAmount = amount;	
}