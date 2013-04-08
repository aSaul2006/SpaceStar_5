#pragma once
#ifndef _ENEMYSHIP_H
#define _ENEMYSHIP_H

#include <d3d9.h>
#include <d3dx9.h>
#include "State, Message, and Entity Systems\State.h"
#include "AABB.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}


class baseEnemyShip //: BaseGameEntity
{
private:

	D3DXMATRIX scaleMat, rotateMat, translateMat, worldMat;
	D3DXVECTOR3 position, velocity;
	ID3DXMesh*	mesh;
	LPDIRECT3DTEXTURE9* texture;
	ID3DXBuffer* materialBuff;
	DWORD numMaterials;
	D3DXMATERIAL* D3DXMaterial;
	D3DMATERIAL9* modelMaterial;
	int health, maxHealth;
	float rotateAngle;
	AABB meshBox;	// mesh's collision box
	//All enemies will need to have a pointer to an instance of a state
	//State* 		m_pCurrentState;

	bool isHealthZero;

public:
	//intializes the enemy ship
	virtual void initializeEnemyShip(IDirect3DDevice9* m_pD3DDevice, LPCWSTR fileName);
	virtual void fireWeapon(int fireRate);
	//Make updateAI and updatePhysics overridable but leave
	//default update() the same...May not work right ha
	void update(float dt); 
	virtual void calculateDamage();
	virtual void destroyShip();
	virtual void setPositionAndVelocity(D3DXVECTOR3 position, D3DXVECTOR3 velocity);
	void Render(ID3DXEffect* shader);
	virtual ~baseEnemyShip();
	void Shutdown();
	
	//methods required for default enemy ship state behavior
	//virtual void ChangeState(State* pNewState);
};

//define states for ships to use for example pursuePlayer, avoidTarget.
class attackPlayer1 : public State<baseEnemyShip>
{
private:
	attackPlayer1(){}
	//copy ctor and assignment 
	attackPlayer1(const attackPlayer1&);
	attackPlayer1 & operator=(const attackPlayer1&);
public:
	//will always be a singleton class
	static attackPlayer1* Instance();
	void Enter(baseEnemyShip* baseEnemy);
	bool Execute(baseEnemyShip* baseEnemy);
	void Exit(baseEnemyShip* baseEnemy);
	bool OnMessage(baseEnemyShip* agent, const Message& msg);
};

class attackPlayer2 : public State<baseEnemyShip>
{
private:
	attackPlayer2(){}
	//copy ctor and assignment 
	attackPlayer2(const attackPlayer2&);
	attackPlayer2 & operator=(const attackPlayer2&);
public:
	//will always be a singleton class
	static attackPlayer2* Instance();
	void Enter(baseEnemyShip* baseEnemy);
	bool Execute(baseEnemyShip* baseEnemy);
	void Exit(baseEnemyShip* baseEnemy);
	bool OnMessage(baseEnemyShip* agent, const Message& msg);
};

class attackPlayer3 : public State<baseEnemyShip>
{
private:
	attackPlayer3(){}
	//copy ctor and assignment 
	attackPlayer3(const attackPlayer3&);
	attackPlayer3 & operator=(const attackPlayer3&);
public:
	//will always be a singleton class
	static attackPlayer3* Instance();
	void Enter(baseEnemyShip* baseEnemy);
	bool Execute(baseEnemyShip* baseEnemy);
	void Exit(baseEnemyShip* baseEnemy);
	bool OnMessage(baseEnemyShip* agent, const Message& msg);
};

class avoidPlayer1 : public State<baseEnemyShip>
{
private:
	avoidPlayer1(){}
	avoidPlayer1(const avoidPlayer1&);
	avoidPlayer1 & operator=(const avoidPlayer1&);
public:
	static avoidPlayer1* Instance();
	void Enter(baseEnemyShip* baseEnemy);
	bool Execute(baseEnemyShip* baseEnemy);
	void Exit(baseEnemyShip* baseEnemy);
	bool OnMessage(baseEnemyShip* agent, const Message& msg);
};
//create ship classes that derive baseEnemyShip

#endif