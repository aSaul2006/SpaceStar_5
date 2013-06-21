#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <list>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "Camera.h"
#include "Initializer.h"

// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

enum ProjectileType
	{
	DEFAULT_BULLET,
	MISSILE1,
	BLAST_RADIUS
	};


//-----------------------------------------------------------------------------
//************************* Abstract Projectile *******************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

class Projectile
{
protected:
	D3DXMATRIX scaleMat, rotateMat, translateMat, worldMat;
	D3DXVECTOR3 position, startPosition, direction;

	AABB meshBox;		// mesh's collision box
	bool destroyObject;	// use this bool to determine when to destroy the object
	ProjectileType projectileType;
	bool isBlastMesh;
	float blastRadiusScale;
	double radiusTimeTrack;
	bool isExpanding;

public:

	// Default constructor
	Projectile(void);
	virtual ~Projectile(void);

	virtual void Initialize()=0;
	virtual void Update(float dt)=0;
	virtual void Render(ID3DXEffect* shader)=0;
	virtual void Shutdown()=0;

	// Checks to see if the object needs to be destroyed
	// True - destroy object; False - keep object alive
	bool CheckObject(void) {return destroyObject;}

	// Accessors
	AABB GetMeshBox()
	{
		AABB out;
		meshBox.xform(worldMat, out);
		return out;
	}							// get collision box

	D3DXVECTOR3 GetPosition(){return position;}

	// Mutators
	void SetPosition(D3DXVECTOR3 position){ this->position = position;}
	void SetStartPosition(D3DXVECTOR3 position){startPosition = position;}
	void SetDirection(D3DXVECTOR3 direction){this->direction = direction;}
	void SetScale(float scale)
	{
		D3DXMatrixScaling(&scaleMat,scale,scale,scale);
	}
	void Destroy() {destroyObject = true;}
	ProjectileType GetProjectileType() {return this->projectileType;}

	//Blast Radius stuff
	void SetIsBlastMesh(bool value) {this->isBlastMesh = value;}
	bool GetIsBlastMesh(){return isBlastMesh;}
	float GetRadiusScale(){return blastRadiusScale;}
	void DecrRadiusScale(float value){this->blastRadiusScale -= value;}
	void IncrRadiusScale(float value){this->blastRadiusScale += value;}
	void ExpandRadius(){isExpanding = true;}
	void ContractRadius(){isExpanding = false;}
	bool GetIsExpanding(){return isExpanding;}

};

//-----------------------------------------------------------------------------
//************************* Default Bullet Class ******************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

class DefaultBullet : public Projectile
{
private:

public:
	// Default constructor
	DefaultBullet(void);

	~DefaultBullet(void);

	void Initialize();
	void Update(float dt);
	void Render(ID3DXEffect* shader);
	void Shutdown();

	// Spawn position is the position of the object firing the projectile
	// Direction in the direction the projectile will be traveling
	DefaultBullet(D3DXVECTOR3 spawnPosition, D3DXVECTOR3 direction);
	DefaultBullet(D3DXVECTOR3 spawnPosition, D3DXVECTOR3 direction, float scale);
	DefaultBullet(D3DXVECTOR3 spawnPosition, float scale);

};

//-----------------------------------------------------------------------------
//************************* Missile Class *************************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

class Missile1 : public Projectile
{
private:
	float m_rotateAngle;

public:
	// Default constructor
	Missile1(void);

	~Missile1(void);

	void Initialize();
	void Update(float dt);
	void Render(ID3DXEffect* shader);
	void Shutdown();

	// Spawn position is the position of the object firing the projectile
	// Direction in the direction the projectile will be traveling
	Missile1(D3DXVECTOR3 spawnPosition, D3DXVECTOR3 direction);
	Missile1(D3DXVECTOR3 spawnPos, D3DXVECTOR3 dir, float scale);

};

//-----------------------------------------------------------------------------
//************************* Blast Radius class  *******************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

class BlastRadius : public Projectile
{
private:
	float m_rotateAngle;
public:
	// Default constructor
	BlastRadius(void);

	~BlastRadius(void);

	void Initialize();
	void Update(float dt);
	void Render(ID3DXEffect* shader);
	void Shutdown();

	//spawn position of blast radius, most likely from projectile position
	//initial scale of object
	BlastRadius(D3DXVECTOR3 spawnPosition, float scale);

};