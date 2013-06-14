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
	Fighter* fighter2 = new Fighter();
	Fighter* fighter3 = new Fighter();
	Viper* viper2 = new Viper();
	Viper* viper3 = new Viper();
	Viper* viper4 = new Viper();
	Viper* viper5 = new Viper();
	Viper* viper6 = new Viper();
	Viper* viper7 = new Viper();
	Viper* viper8 = new Viper();
	Viper* viper9 = new Viper();

	//Ypos can't go above +5.0 or below -7.0
	fighter1->SetEnemyAttrib2(200,2.0,2.0,ATTACK2,D3DXVECTOR3(12.0,4.0,0.0));
	fighter2->SetEnemyAttrib2(200,4.0,2.0,ATTACK2,D3DXVECTOR3(48.0,3.0,0.0));
	fighter3->SetEnemyAttrib2(200,4.0,2.0,ATTACK1,D3DXVECTOR3(72.0,4.0,0.0));
	
	viper2->SetEnemyAttrib2(100,4.0,1,ATTACK1,D3DXVECTOR3(24.0,2.0,0.0));
	viper3->SetEnemyAttrib2(100,4.0,10,ATTACK3,D3DXVECTOR3(36.0,0.0,0.0));
	viper4->SetEnemyAttrib2(100,4.0,10,ATTACK1,D3DXVECTOR3(48.0,0.0,0.0));
	viper5->SetEnemyAttrib2(100,6.0,5,AVOID1,D3DXVECTOR3(60.0,5.0,0.0));
	viper6->SetEnemyAttrib2(100,4.0,5,ATTACK1,D3DXVECTOR3(24.0,-2.0,0.0));
	viper7->SetEnemyAttrib2(100,4.0,5,ATTACK3,D3DXVECTOR3(30.0,-3.0,0.0));
	viper8->SetEnemyAttrib2(100,4.0,5,ATTACK1,D3DXVECTOR3(48.0,-4.0,0.0));
	viper9->SetEnemyAttrib2(100,7.0,1,AVOID2,D3DXVECTOR3(60.0,-5.0,0.0));

	enemies.push_front(fighter1); enemies.push_front(fighter2); enemies.push_front(fighter3);

	enemies.push_front(viper2); enemies.push_front(viper3);
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

	Scooter* scoot1 = new Scooter();
	Scooter* scoot2 = new Scooter();
	Scooter* scoot3 = new Scooter();
	Scooter* scoot4 = new Scooter();
	Scooter* scoot5 = new Scooter();

	Fighter* fighter1 = new Fighter();

	viper1->SetEnemyAttrib2(100,7.0,10,ATTACK2,D3DXVECTOR3(12.0,0.0,0.0));
	viper2->SetEnemyAttrib2(100,7.0,10,ATTACK2,D3DXVECTOR3(28.0,0.0,0.0));
	viper3->SetEnemyAttrib2(100,7.0,10,ATTACK2,D3DXVECTOR3(44.0,0.0,0.0));
	viper4->SetEnemyAttrib2(100,7.0,10,ATTACK2,D3DXVECTOR3(60.0,0.0,0.0));
	viper5->SetEnemyAttrib2(100,7.0,5,ATTACK2,D3DXVECTOR3(76.0,0.0,0.0));
	viper6->SetEnemyAttrib2(100,7.0,5,ATTACK5,D3DXVECTOR3(20.0,0.0,0.0));
	viper7->SetEnemyAttrib2(100,5.0,5,ATTACK5,D3DXVECTOR3(82.0,0.0,0.0));
	viper8->SetEnemyAttrib2(100,5.0,5,ATTACK5,D3DXVECTOR3(88.0,0.0,0.0));
	viper9->SetEnemyAttrib2(100,5.0,10,ATTACK5,D3DXVECTOR3(94.0,0.0,0.0));
	viper10->SetEnemyAttrib2(100,5.0,10,ATTACK5,D3DXVECTOR3(100.0,0.0,0.0));

	scoot1->SetEnemyAttrib2(50,5.0,10,ATTACK5,D3DXVECTOR3(36.0,4.0,0.0));
	scoot2->SetEnemyAttrib2(50,5.0,10,ATTACK5,D3DXVECTOR3(52.0,-3.0,0.0));
	scoot3->SetEnemyAttrib2(50,5.0,10,ATTACK5,D3DXVECTOR3(68.0,2.0,0.0));
	scoot4->SetEnemyAttrib2(50,5.0,10,ATTACK5,D3DXVECTOR3(112,0.0,0.0));
	scoot5->SetEnemyAttrib2(50,5.0,10,ATTACK5,D3DXVECTOR3(116,-6.0,0.0));

	fighter1->SetEnemyAttrib2(250,4.0,2.0,ATTACK2,D3DXVECTOR3(80.0,5.0,0.0));

	enemies.push_front(fighter1);

	enemies.push_front(viper1);enemies.push_front(viper2); enemies.push_front(viper3);
	enemies.push_front(viper4);enemies.push_front(viper5); enemies.push_front(viper6);
	enemies.push_front(viper7);enemies.push_front(viper8); enemies.push_front(viper9);

	enemies.push_front(scoot1);enemies.push_front(scoot2);enemies.push_front(scoot3);
	enemies.push_front(scoot4);enemies.push_front(scoot5);
	
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
	Viper* viper10 = new Viper();

	Scooter* scoot1 = new Scooter();
	Scooter* scoot2 = new Scooter();
	Scooter* scoot3 = new Scooter();
	Scooter* scoot4 = new Scooter();

	viper1->SetEnemyAttrib2(100,5.0,1.0,ATTACK3,D3DXVECTOR3(12,0,0));
	viper2->SetEnemyAttrib2(100,5.0,1.0,ATTACK5,D3DXVECTOR3(18,-3,0));
	viper3->SetEnemyAttrib2(100,5.0,1.0,ATTACK2,D3DXVECTOR3(18,3,0));
	viper4->SetEnemyAttrib2(100,5.0,1.0,AVOID1,D3DXVECTOR3(35,5,0));
	viper5->SetEnemyAttrib2(100,5.0,1.0,AVOID2,D3DXVECTOR3(35,-7,0));
	viper6->SetEnemyAttrib2(100,5.0,1.0,ATTACK4,D3DXVECTOR3(42,-7,0));
	viper7->SetEnemyAttrib2(150,7.0,0.5,ATTACK3,D3DXVECTOR3(80,0,0));
	viper8->SetEnemyAttrib2(150,7.0,0.5,ATTACK5,D3DXVECTOR3(90,0,0));
	viper9->SetEnemyAttrib2(150,7.0,0.5,ATTACK2,D3DXVECTOR3(100,2,0));
	viper10->SetEnemyAttrib2(150,7.0,0.5,ATTACK4,D3DXVECTOR3(100,-4,0));

	scoot1->SetEnemyAttrib2(50,5.0,0.5,ATTACK5,D3DXVECTOR3(25,4,0));
	scoot2->SetEnemyAttrib2(50,5.0,0.5,ATTACK5,D3DXVECTOR3(25,-7,0));
	scoot3->SetEnemyAttrib2(50,5.0,0.5,ATTACK5,D3DXVECTOR3(50,0,0));
	scoot4->SetEnemyAttrib2(50,5.0,0.5,ATTACK5,D3DXVECTOR3(55,0,0));

	enemies.push_front(viper1);enemies.push_front(viper2); enemies.push_front(viper3);
	enemies.push_front(viper4);enemies.push_front(viper5); enemies.push_front(viper6);
	enemies.push_front(viper7);enemies.push_front(viper8); enemies.push_front(viper9);
	enemies.push_front(viper10);

	enemies.push_front(scoot1);enemies.push_front(scoot2);enemies.push_front(scoot3);enemies.push_front(scoot4);
	
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