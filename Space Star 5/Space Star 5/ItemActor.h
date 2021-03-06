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
#include "ItemTypes.h"

class ItemActor
{
protected:
	D3DXMATRIX scaleMat, rotateMat, translateMat, worldMat;
	D3DXVECTOR3 m_position;
	D3DXVECTOR3	m_rotateAngle;
	int health, maxHealth;
	float m_speed;
	AABB meshBox;	// mesh's collision box
	FMOD::Sound* itemSFX;
	bool destroyObject;
	ItemType typeOfItemDropped;

public:

	/*
	 *Initialize the Mesh object
	 */
	virtual void Initialize()=0;

	/*
	 * Update <Item> object
	 */
	virtual void Update(float dt)=0;

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
	void DropItem(std::list<ItemActor*> &item, D3DXVECTOR3 pos);

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

	virtual void DestroyItem(bool value) { destroyObject = value;}

	ItemType getItemType() {return typeOfItemDropped;}
	
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
	HealthItemActor(D3DXVECTOR3 pos);
	//destructor
	~HealthItemActor(void);

	void Update(float dt);
	void Render(ID3DXEffect* shader);
	void Initialize(void);
};


//-----------------------------------------------------------------------------
//************************* StarDust Pick up class ***************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

class StarDustItemActor : public ItemActor
{
private:
	float xpos;
	float ypos;
public:
	//constructor
	StarDustItemActor();
	//destructor
	~StarDustItemActor(void);

	void Update(float dt);
	void Render(ID3DXEffect* shader);
	void Initialize(void);
	//void SetXPosition(float xpos){this->xpos = xpos;}
	//void SetYPosition(float ypos){this->ypos = ypos;}
	void SetPosition()
	{
		m_position = D3DXVECTOR3(this->xpos, this->ypos,0.0);
	}
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
	Missile1ItemActor(D3DXVECTOR3 pos);
	//destructor
	~Missile1ItemActor(void);

	void Update(float dt);
	void Render(ID3DXEffect* shader);
	void Initialize(void);
};
#endif
