// This base class will be inherited by other classes implementing
// Items dropped by enemies

#pragma once
#ifndef _ITEMACTOR_H
#define _ITEMACTOR_H

#include <d3d9.h>
#include <d3dx9.h>
#include <list>
#include "AABB.h"
#include "AudioManager.h"
#include "EnemyShip.h"

// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

class ItemActor
{
protected:
	D3DXMATRIX scaleMat, rotateMat, translateMat, worldMat;
	D3DXVECTOR3 m_position;
	int health, maxHealth;
	float m_speed, m_rotateAngle;
	AABB meshBox;	// mesh's collision box
	FMOD::Sound* itemSFX;
	bool destroyObject;
public:

	/*
	 *Initialize the Mesh object
	 */
	virtual void Initialize()=0;

	/*
	 * Update <Item> object
	 */
	virtual void Update(float dt, baseEnemyShip* enemy)=0;

	/*
	 * Render <Item> object
	 */
	virtual void Render(ID3DXEffect* shader)=0;
	
	/*
	 * Clean up mess
	 */
	void Shutdown(void);

	/*
	 * Choose a random pickup item to drop
	 * @Returns a pointer to <ItemActor*> object initialized 
	 */
	virtual ItemActor* DropItem()=0;

	//Constructor/Destructor
	ItemActor(void);
	virtual ~ItemActor(void);

	//Accessors
	/*
	 * Check if Object is set to be destroyed
	 */
	virtual bool CheckObject(void) {return destroyObject;}
	/*
	 * get the Position of the item
	 */
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
//************************* Health Pick up class ******************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

class HealthItemActor : public ItemActor
{
private:

public:
	//constructor 
	HealthItemActor(void);
	//destructor
	~HealthItemActor(void);

	ItemActor* DropItem(void);
	void Update(float dt, baseEnemyShip* enemy);
	void Render(ID3DXEffect* shader);
	void Initialize(void);
};

//-----------------------------------------------------------------------------
//************************* Missile 1 Pick up class ***************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

class Missile1ItemActor : public ItemActor
{
private:

public:
	//constructor
	Missile1ItemActor(void);
	//destructor
	~Missile1ItemActor(void);

	ItemActor* DropItem(void);
	void Update(float dt, baseEnemyShip* enemy);
	void Render(ID3DXEffect* shader);
	void Initialize(void);
};
#endif
