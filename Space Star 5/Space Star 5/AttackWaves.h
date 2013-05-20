#ifndef _ATTACKWAVES_H
#define _ATTACKWAVES_H

#include "EnemyShip.h"
#include "Initializer.h"
#include "AttackType.h"
#include <list>

using namespace std;

#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}

class AttackWaves
{
protected:
	AttackType at;
	
public:
	AttackWaves(void);
	~AttackWaves(void);
	virtual void AttackPattern(list<baseEnemyShip*> &enemies)=0;
	virtual void Shutdown(void)=0;

};

class ViperWave1 : public AttackWaves
{
private:
	int enemyAmount;
public:
	~ViperWave1(void);
	ViperWave1(void);
	void AttackPattern(list<baseEnemyShip*> &enemies);
	void Shutdown(void);
};

class ViperWave2 : public AttackWaves
{
private:
	int enemyAmount;
public:
	~ViperWave2(void);
	ViperWave2(void);
	void AttackPattern(list<baseEnemyShip*> &enemies);
	void Shutdown(void);
};

class ViperWave3 : public AttackWaves
{
private:
	int enemyAmount;
public:
	~ViperWave3(void);
	ViperWave3(void);
	void AttackPattern(list<baseEnemyShip*> &enemies);
	void Shutdown(void);
};

class ViperWave4 : public AttackWaves
{
private:
	int enemyAmount;
public:
	~ViperWave4(void);
	ViperWave4(void);
	void AttackPattern(list<baseEnemyShip*> &enemies);
	void Shutdown(void);
};

class ViperWave5 : public AttackWaves
{
private:
	int enemyAmount;
public:
	~ViperWave5(void);
	ViperWave5(void);
	void AttackPattern(list<baseEnemyShip*> &enemies);
	void Shutdown(void);
};
#endif
