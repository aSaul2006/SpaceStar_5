#include "AttackWaves.h"
using namespace std;

template< class T > void SafeDelete( T*& pVal )
{
    delete pVal;
    pVal = NULL;
}

//Base Class
AttackWaves::AttackWaves(void)
{

}
AttackWaves::~AttackWaves(void)
{

}

//Viper Wave 1
ViperWave1::~ViperWave1()
{
	Shutdown();
}

ViperWave1::ViperWave1()
{
	enemyAmount = 0;
}

void ViperWave1::Shutdown()
{
	enemyAmount = NULL;
}

void ViperWave1::AttackPattern(list<baseEnemyShip*> &enemies)
{
	Fighter* fighter1 = new Fighter();
	Viper* viper2 = new Viper();
	Viper* viper3 = new Viper();
	Viper* viper4 = new Viper();
	Viper* viper5 = new Viper();
	Viper* viper6 = new Viper();
	Viper* viper7 = new Viper();
	Viper* viper8 = new Viper();
	Viper* viper9 = new Viper();

	fighter1->SetEnemyAttrib2(100,2.0,0.5,ATTACK1,D3DXVECTOR3(12.0,5.0,0.0));
	viper2->SetEnemyAttrib2(100,4.0,1,ATTACK1,D3DXVECTOR3(24.0,2.0,0.0));
	viper3->SetEnemyAttrib2(100,4.0,0.5,ATTACK3,D3DXVECTOR3(36.0,0.0,0.0));
	viper4->SetEnemyAttrib2(100,4.0,0.5,ATTACK1,D3DXVECTOR3(48.0,4.0,0.0));
	viper5->SetEnemyAttrib2(100,6.0,1,AVOID1,D3DXVECTOR3(60.0,5.0,0.0));
	viper6->SetEnemyAttrib2(100,4.0,1,ATTACK1,D3DXVECTOR3(24.0,-2.0,0.0));
	viper7->SetEnemyAttrib2(100,4.0,0.5,ATTACK3,D3DXVECTOR3(30.0,-3.0,0.0));
	viper8->SetEnemyAttrib2(100,4.0,0.5,ATTACK1,D3DXVECTOR3(48.0,-4.0,0.0));
	viper9->SetEnemyAttrib2(100,7.0,1,AVOID2,D3DXVECTOR3(60.0,-5.0,0.0));

	enemies.push_front(fighter1);enemies.push_front(viper2); enemies.push_front(viper3);
	enemies.push_front(viper4);enemies.push_front(viper5); enemies.push_front(viper6);
	enemies.push_front(viper7);enemies.push_front(viper8); enemies.push_front(viper9);
		
}

ViperWave2::~ViperWave2()
{
	Shutdown();
}

ViperWave2::ViperWave2()
{
	enemyAmount = 0;
}

void ViperWave2::Shutdown()
{
	enemyAmount = NULL;
}

void ViperWave2::AttackPattern(list<baseEnemyShip*> &enemies)
{
	Viper* viper1 = new Viper();
	Viper* viper2 = new Viper();
	Viper* viper3 = new Viper();
	Viper* viper4 = new Viper();
	Viper* viper5 = new Viper();
	Viper* viper6 = new Viper();
	Viper* viper7 = new Viper();
	Viper* viper8 = new Viper();
	Viper* viper9 = new Viper();
	Viper* viper10 = new Viper();

	viper1->SetEnemyAttrib2(100,7.0,1.0,ATTACK2,D3DXVECTOR3(12.0,0.0,0.0));
	viper2->SetEnemyAttrib2(100,7.0,1.0,ATTACK2,D3DXVECTOR3(28.0,0.0,0.0));
	viper3->SetEnemyAttrib2(100,7.0,1.0,ATTACK2,D3DXVECTOR3(44.0,0.0,0.0));
	viper4->SetEnemyAttrib2(100,7.0,1.0,ATTACK2,D3DXVECTOR3(60.0,0.0,0.0));
	viper5->SetEnemyAttrib2(100,7.0,0.5,ATTACK2,D3DXVECTOR3(76.0,0.0,0.0));
	viper6->SetEnemyAttrib2(100,7.0,0.5,ATTACK5,D3DXVECTOR3(20.0,0.0,0.0));
	viper7->SetEnemyAttrib2(100,7.0,0.5,ATTACK5,D3DXVECTOR3(36.0,0.0,0.0));
	viper8->SetEnemyAttrib2(100,7.0,0.5,ATTACK5,D3DXVECTOR3(52.0,0.0,0.0));
	viper9->SetEnemyAttrib2(100,7.0,1.0,ATTACK5,D3DXVECTOR3(68.0,0.0,0.0));
	viper10->SetEnemyAttrib2(100,7.0,1.0,ATTACK5,D3DXVECTOR3(82.0,0.0,0.0));

	enemies.push_front(viper1);enemies.push_front(viper2); enemies.push_front(viper3);
	enemies.push_front(viper4);enemies.push_front(viper5); enemies.push_front(viper6);
	enemies.push_front(viper7);enemies.push_front(viper8); enemies.push_front(viper9);
	
}

ViperWave3::~ViperWave3()
{
	Shutdown();
}

ViperWave3::ViperWave3()
{
	enemyAmount = 0;
}

void ViperWave3::Shutdown()
{
	enemyAmount = NULL;
}

void ViperWave3::AttackPattern(list<baseEnemyShip*> &enemies)
{
	Viper* viper1 = new Viper();
	Viper* viper2 = new Viper();
	Viper* viper3 = new Viper();
	Viper* viper4 = new Viper();
	Viper* viper5 = new Viper();
	Viper* viper6 = new Viper();
	Viper* viper7 = new Viper();
	Viper* viper8 = new Viper();
	Viper* viper9 = new Viper();

	viper1->SetEnemyAttrib2(100,3.0,0.5,ATTACK3,D3DXVECTOR3(12.0,0.0,0.0));
	viper2->SetEnemyAttrib2(100,4.0,1,ATTACK1,D3DXVECTOR3(24.0,2.0,0.0));
	viper3->SetEnemyAttrib2(100,4.0,0.5,ATTACK3,D3DXVECTOR3(36.0,0.0,0.0));
	viper4->SetEnemyAttrib2(100,4.0,0.5,ATTACK1,D3DXVECTOR3(48.0,4.0,0.0));
	viper5->SetEnemyAttrib2(100,6.0,1,AVOID1,D3DXVECTOR3(60.0,5.0,0.0));
	viper6->SetEnemyAttrib2(100,4.0,1,ATTACK1,D3DXVECTOR3(24.0,-2.0,0.0));
	viper7->SetEnemyAttrib2(100,4.0,0.5,ATTACK3,D3DXVECTOR3(30.0,-3.0,0.0));
	viper8->SetEnemyAttrib2(100,4.0,0.5,ATTACK1,D3DXVECTOR3(48.0,-4.0,0.0));
	viper9->SetEnemyAttrib2(100,7.0,1,AVOID2,D3DXVECTOR3(60.0,-5.0,0.0));

	enemies.push_front(viper1);enemies.push_front(viper2); enemies.push_front(viper3);
	enemies.push_front(viper4);enemies.push_front(viper5); enemies.push_front(viper6);
	enemies.push_front(viper7);enemies.push_front(viper8); enemies.push_front(viper9);
	
}

ViperWave4::~ViperWave4()
{
	Shutdown();
}

ViperWave4::ViperWave4()
{
	enemyAmount = 0;
}

void ViperWave4::Shutdown()
{
	enemyAmount = NULL;
}

void ViperWave4::AttackPattern(list<baseEnemyShip*> &enemies)
{
	Viper* viper1 = new Viper();
	Viper* viper2 = new Viper();
	Viper* viper3 = new Viper();
	Viper* viper4 = new Viper();
	Viper* viper5 = new Viper();
	Viper* viper6 = new Viper();
	Viper* viper7 = new Viper();
	Viper* viper8 = new Viper();
	Viper* viper9 = new Viper();

	viper1->SetEnemyAttrib2(100,3.0,0.5,ATTACK3,D3DXVECTOR3(12.0,0.0,0.0));
	viper2->SetEnemyAttrib2(100,4.0,1,ATTACK1,D3DXVECTOR3(24.0,2.0,0.0));
	viper3->SetEnemyAttrib2(100,4.0,0.5,ATTACK3,D3DXVECTOR3(36.0,0.0,0.0));
	viper4->SetEnemyAttrib2(100,4.0,0.5,ATTACK1,D3DXVECTOR3(48.0,4.0,0.0));
	viper5->SetEnemyAttrib2(100,6.0,1,AVOID1,D3DXVECTOR3(60.0,5.0,0.0));
	viper6->SetEnemyAttrib2(100,4.0,1,ATTACK1,D3DXVECTOR3(24.0,-2.0,0.0));
	viper7->SetEnemyAttrib2(100,4.0,0.5,ATTACK3,D3DXVECTOR3(30.0,-3.0,0.0));
	viper8->SetEnemyAttrib2(100,4.0,0.5,ATTACK1,D3DXVECTOR3(48.0,-4.0,0.0));
	viper9->SetEnemyAttrib2(100,7.0,1,AVOID2,D3DXVECTOR3(60.0,-5.0,0.0));

	enemies.push_front(viper1);enemies.push_front(viper2); enemies.push_front(viper3);
	enemies.push_front(viper4);enemies.push_front(viper5); enemies.push_front(viper6);
	enemies.push_front(viper7);enemies.push_front(viper8); enemies.push_front(viper9);
	
}

ViperWave5::~ViperWave5()
{
	Shutdown();
}

ViperWave5::ViperWave5()
{
	enemyAmount = 0;
}

void ViperWave5::Shutdown()
{
	enemyAmount = NULL;
}

void ViperWave5::AttackPattern(list<baseEnemyShip*> &enemies)
{
	Viper* viper1 = new Viper();
	Viper* viper2 = new Viper();
	Viper* viper3 = new Viper();
	Viper* viper4 = new Viper();
	Viper* viper5 = new Viper();
	Viper* viper6 = new Viper();
	Viper* viper7 = new Viper();
	Viper* viper8 = new Viper();
	Viper* viper9 = new Viper();

	viper1->SetEnemyAttrib2(100,3.0,0.5,ATTACK3,D3DXVECTOR3(12.0,0.0,0.0));
	viper2->SetEnemyAttrib2(100,4.0,1,ATTACK1,D3DXVECTOR3(24.0,2.0,0.0));
	viper3->SetEnemyAttrib2(100,4.0,0.5,ATTACK3,D3DXVECTOR3(36.0,0.0,0.0));
	viper4->SetEnemyAttrib2(100,4.0,0.5,ATTACK1,D3DXVECTOR3(48.0,4.0,0.0));
	viper5->SetEnemyAttrib2(100,6.0,1,AVOID1,D3DXVECTOR3(60.0,5.0,0.0));
	viper6->SetEnemyAttrib2(100,4.0,1,ATTACK1,D3DXVECTOR3(24.0,-2.0,0.0));
	viper7->SetEnemyAttrib2(100,4.0,0.5,ATTACK3,D3DXVECTOR3(30.0,-3.0,0.0));
	viper8->SetEnemyAttrib2(100,4.0,0.5,ATTACK1,D3DXVECTOR3(48.0,-4.0,0.0));
	viper9->SetEnemyAttrib2(100,7.0,1,AVOID2,D3DXVECTOR3(60.0,-5.0,0.0));

	enemies.push_front(viper1);enemies.push_front(viper2); enemies.push_front(viper3);
	enemies.push_front(viper4);enemies.push_front(viper5); enemies.push_front(viper6);
	enemies.push_front(viper7);enemies.push_front(viper8); enemies.push_front(viper9);
	
}