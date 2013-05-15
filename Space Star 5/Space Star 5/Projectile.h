#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "Camera.h"
#include "Initializer.h"

// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

class Projectile
{
private:
	D3DXMATRIX scaleMat, rotateMat, translateMat, worldMat;
	D3DXVECTOR3 position, startPosition, direction;

	AABB meshBox;		// mesh's collision box
	bool destroyObject;	// use this bool to determine when to destroy the object
public:
	// Default constructor
	Projectile(void);

	// Spawn position is the position of the object firing the projectile
	// Direction is the direction the projectile will be traveling
	Projectile(D3DXVECTOR3 spawnPosition, D3DXVECTOR3 direction);
	~Projectile(void);

	void Initialize();
	void Update(float dt);
	void Render(ID3DXEffect* shader);
	void Shutdown();

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
	void Destroy() {destroyObject = true;}
};

