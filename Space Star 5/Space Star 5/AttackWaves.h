#include "EnemyShip.h"
#include "Initializer.h"
#include "AttackType.h"
#include <list>

using namespace std;

class AttackWaves
{
protected:
	AttackType at;
	
public:
	AttackWaves(void);
	~AttackWaves(void);
	virtual void SetEnemyAmount(int amount)=0;
	virtual void AttackPattern( AttackType at)=0;
	virtual void Shutdown(void)=0;

};

class ViperWave1 : public AttackWaves
{
private:
	int enemyAmount;
	list<Enemy*> pEnemiesInViperOne;
public:
	~ViperWave1(void);
	ViperWave1(void);
	void SetEnemyAmount(int amount);
	void AttackPattern(AttackType at);
	void Shutdown(void);
};