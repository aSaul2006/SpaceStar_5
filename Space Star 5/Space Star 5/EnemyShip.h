#pragma once
#ifndef _ENEMYSHIP_H
#define _ENEMYSHIP_H

#include <d3d9.h>
#include <d3dx9.h>
#include "AABB.h"
#include "State\State.h"
#include "AttackType.h"
#include "Projectile.h"
#include "Player.h"
#include <list>
#include "AudioManager.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")



// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}


class baseEnemyShip //: BaseGameEntity
{
protected:

	D3DXMATRIX scaleMat, rotateMat, translateMat, worldMat;
	D3DXVECTOR3 m_position, m_velocity;
	int health, maxHealth;
	float m_speed, m_fireRate, m_rotateAngle;
	AABB meshBox;	// mesh's collision box
	AttackType m_attackType;
	//All enemies will need to have a pointer to an instance of a state
	//State* 		m_pCurrentState;
	FMOD::Sound* enemySFX;
	bool isHealthZero;

public:
	//intializes the enemy ship
	void initializeEnemyShip();
	//virtual void fireWeapon(int fireRate)=0;
	//Make updateAI and updatePhysics overridable but leave
	//default update() the same...May not work right ha
	//virtual void update(float dt)=0; 
	virtual void calculateDamage(int power)=0;
	virtual void destroyShip()=0;
	virtual ~baseEnemyShip();
	void Shutdown();
	baseEnemyShip();
	//methods required for default enemy ship state behavior
	//virtual void ChangeState(State* pNewState);
};

//create ship classes that derive baseEnemyShip
class Enemy : public baseEnemyShip
{
private:
	std::list<Projectile*> enemyBullet;
	int track;
	bool moveDir;
	bool destroyObject;
	bool isHidden;
	float angle;

public:
	Enemy();
	~Enemy();

	void Render(ID3DXEffect* shader);

	//Inherited functions
	void update(float dt, Player *player); 
	void calculateDamage(int power);
	void destroyShip();
	void hideShip(bool yn){isHidden = yn;}
	void fireWeapon(int fireRate, Player* player);
	void renderBullet(ID3DXEffect* shader);
	void SetEnemyAttrib(int shipHealth,float speed,float rate, D3DXVECTOR3 pos);
	int getHealth(){return health;}
	bool CheckObject(void) {return destroyObject;}
	//void loadEnemies(std::list<Enemy*> pEnemies, int numberToLoad);
	bool GetIsHidden(){return isHidden;}
	bool hasSpawned;

	D3DXVECTOR3 getPosition(){return m_position;}
	// get collision box
	AABB GetMeshBox()
	{
		AABB out;
		meshBox.xform(worldMat, out);
		return out;
	}
};

#endif