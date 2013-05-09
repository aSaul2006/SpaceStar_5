#include "AttackWaves.h"
using namespace std;


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

void ViperWave1::AttackPattern(list<Enemy*> *enemies)
{
	Enemy* viper1 = new Enemy();
	Enemy* viper2 = new Enemy();
	Enemy* viper3 = new Enemy();
	Enemy* viper4 = new Enemy();
	Enemy* viper5 = new Enemy();
	Enemy* viper6 = new Enemy();
	Enemy* viper7 = new Enemy();
	Enemy* viper8 = new Enemy();
	Enemy* viper9 = new Enemy();

	viper1->initializeEnemyShip(); viper1->SetEnemyAttrib2(100,5.0,ATTACK3,D3DXVECTOR3(12.0,0.0,0.0));
	viper2->initializeEnemyShip(); viper2->SetEnemyAttrib2(100,5.0,ATTACK3,D3DXVECTOR3(12.0,-7.0,0.0));
	viper3->initializeEnemyShip(); viper3->SetEnemyAttrib2(100,5.0,ATTACK3,D3DXVECTOR3(12.0,-7.0,0.0));
	viper4->initializeEnemyShip(); viper4->SetEnemyAttrib2(100,5.0,ATTACK3,D3DXVECTOR3(12.0,-7.0,0.0));
	viper5->initializeEnemyShip(); viper5->SetEnemyAttrib2(100,5.0,ATTACK3,D3DXVECTOR3(12.0,-7.0,0.0));
	viper6->initializeEnemyShip(); viper6->SetEnemyAttrib2(100,5.0,ATTACK3,D3DXVECTOR3(12.0,5.0,0.0));
	viper7->initializeEnemyShip(); viper7->SetEnemyAttrib2(100,5.0,ATTACK3,D3DXVECTOR3(12.0,5.0,0.0));
	viper8->initializeEnemyShip(); viper8->SetEnemyAttrib2(100,5.0,ATTACK3,D3DXVECTOR3(12.0,5.0,0.0));
	viper9->initializeEnemyShip(); viper9->SetEnemyAttrib2(100,5.0,ATTACK3,D3DXVECTOR3(12.0,5.0,0.0));

	enemies->push_front(viper1);enemies->push_front(viper2); enemies->push_front(viper3);
	enemies->push_front(viper4);enemies->push_front(viper5); enemies->push_front(viper6);
	enemies->push_front(viper7);enemies->push_front(viper8); enemies->push_front(viper9);
}

void ViperWave1::SetEnemyAmount(int amount)
{
	enemyAmount = amount;	
}