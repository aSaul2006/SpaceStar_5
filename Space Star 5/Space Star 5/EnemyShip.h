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
	D3DXVECTOR3 m_position;
	ID3DXMesh*	mesh;
	LPDIRECT3DTEXTURE9* texture;
	ID3DXBuffer* materialBuff;
	DWORD numMaterials;
	D3DXMATERIAL* D3DXMaterial;
	D3DMATERIAL9* modelMaterial;
	int health, maxHealth;
	float m_speed, m_fireRate, m_rotateAngle;
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
	void Render(ID3DXEffect* shader);
	virtual ~baseEnemyShip();
	void Shutdown();
	baseEnemyShip();

	void setPosition(D3DXVECTOR3 position);
	void setSpeed(float speed);
	void setFireRate(float rate);
	//methods required for default enemy ship state behavior
	//virtual void ChangeState(State* pNewState);
};



#endif