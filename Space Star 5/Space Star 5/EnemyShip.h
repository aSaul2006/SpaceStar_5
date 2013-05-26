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
	FMOD::Sound* enemySFX;
	bool isHealthZero;
	bool destroyObject;
	bool isHidden;
	double spawnTime;

public:
	//intializes the enemy ship
	virtual void initialize()=0;
	void Shutdown();
	baseEnemyShip();
	virtual void fireWeapon(int fireRate, Player* player)=0;
	virtual void update(float dt, Player *player)=0; 
	virtual void calculateDamage(int power)=0;
	virtual void destroyShip()=0;
	virtual void Render(ID3DXEffect* shader)=0;
	virtual void renderBullet(ID3DXEffect* shader)=0;
	virtual ~baseEnemyShip();

	//Accessors
	virtual void SetEnemyAttrib(int shipHealth, float speed, float rate, D3DXVECTOR3 pos)=0;
	virtual void SetEnemyAttrib2(int shipHealth,float speed,AttackType at, D3DXVECTOR3 pos)=0;
	void hideShip(bool yn){isHidden = yn;}

	virtual bool GetIsHidden(){return isHidden;}
	virtual int getHealth(){return health;}
	virtual bool CheckObject(void) {return destroyObject;}
	virtual D3DXVECTOR3 getPosition(){return m_position;}
	// get collision box
	AABB GetMeshBox()
	{
		AABB out;
		meshBox.xform(worldMat, out);
		return out;
	}

	
};

//-----------------------------------------------------------------------------
//************************* Viper Class ***************************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

class Viper : public baseEnemyShip
{
private:
	std::list<Projectile*> enemyBullet;
	int track;
	bool moveDir;
	float angle;

public:
	bool hasSpawned;

	Viper();
	~Viper();

	//Inherited functions
	/* 
	 *
	 */
	void Render(ID3DXEffect* shader);
	/*
	 * 
	 */
	void update(float dt, Player *player); 
	/*
	 *
	 */
	void calculateDamage(int power){health -= power;}
	/*
	 *
	 */
	void destroyShip(){destroyObject = true;}
	/*
	 *
	 */
	void fireWeapon(int fireRate, Player* player);
	/*
	 *
	 */
	void renderBullet(ID3DXEffect* shader);
	/*
	 *
	 */
	void initialize();
		
	//Accessors
	void SetEnemyAttrib(int shipHealth,float speed,float rate, D3DXVECTOR3 pos);
	void SetEnemyAttrib2(int shipHealth,float speed,AttackType at, D3DXVECTOR3 pos);


};

//-----------------------------------------------------------------------------
//************************* Scooter Class *************************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

class Scooter : public baseEnemyShip
{
private:
	std::list<Projectile*> enemyBullet;
	int track;
	bool moveDir;
	float angle;

public:
	bool hasSpawned;

	Scooter();
	~Scooter();

	//Inherited functions
	/*
	 *
	 */
	void Render(ID3DXEffect* shader);
	/*
	 *
	 */
	void update(float dt, Player *player); 
	/*
	 *
	 */
	void calculateDamage(int power){health -= power;}
	/*
	 *
	 */
	void destroyShip(){destroyObject = true;}
	/*
	 *
	 */
	void fireWeapon(int fireRate, Player* player);
	/*
	 *
	 */
	void renderBullet(ID3DXEffect* shader);
	/*
	 *
	 */
	void initialize();
		
	//Accessors
	void SetEnemyAttrib(int shipHealth,float speed,float rate, D3DXVECTOR3 pos);
	void SetEnemyAttrib2(int shipHealth,float speed,AttackType at, D3DXVECTOR3 pos);
};

//-----------------------------------------------------------------------------
//************************* Fighter Class *************************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

class Fighter : public baseEnemyShip
{
private:
	std::list<Projectile*> enemyBullet;
	int track;
	bool moveDir;
	float angle;

public:
	bool hasSpawned;

	Fighter();
	~Fighter();

	//Inherited functions
	/*
	 *
	 */
	void Render(ID3DXEffect* shader);
	/*
	 *
	 */
	void update(float dt, Player *player); 
	/*
	 *
	 */
	void calculateDamage(int power){health -= power;}
	/*
	 *
	 */
	void destroyShip(){destroyObject = true;}
	/*
	 *
	 */
	void fireWeapon(int fireRate, Player* player);
	/*
	 *
	 */
	void renderBullet(ID3DXEffect* shader);
	/*
	 *
	 */
	void initialize();
		
	//Accessors
	void SetEnemyAttrib(int shipHealth,float speed,float rate, D3DXVECTOR3 pos);
	void SetEnemyAttrib2(int shipHealth,float speed,AttackType at, D3DXVECTOR3 pos);
};

#endif