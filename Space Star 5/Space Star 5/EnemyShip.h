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
	
public:
};

class attackPlayer2 : public State<baseEnemyShip>
{
private:
	
public:
};

class attackPlayer3 : public State<baseEnemyShip>
{
private:

public:
};

class avoidPlayer1 : public State<baseEnemyShip>
{
private:
public:
};
//create ship classes that derive baseEnemyShip

#endif