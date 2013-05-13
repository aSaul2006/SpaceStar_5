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
	virtual void SetEnemyAmount(int amount)=0;
	virtual void AttackPattern(list<Enemy*> *enemies)=0;
	virtual void Shutdown(void)=0;

};

class ViperWave1 : public AttackWaves
{
private:
	int enemyAmount;
public:
	~ViperWave1(void);
	ViperWave1(void);
	void SetEnemyAmount(int amount);
	void AttackPattern(list<Enemy*> *enemies);
	void Shutdown(void);
};