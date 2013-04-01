#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}


class baseEnemyShip
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

	bool isHealthZero;

public:
	//intializes the enemy ship
	virtual void initializeEnemyShip(IDirect3DDevice9* m_pD3DDevice, LPCWSTR fileName);
	virtual void fireWeapon(int fireRate);
	virtual void update(float dt); 
	virtual void calculateDamage();
	virtual void destroyShip();
	virtual void setPositionAndVelocity(D3DXVECTOR3 position, D3DXVECTOR3 velocity);
	virtual ~baseEnemyShip();
	void Shutdown();
};

//create ship classes that derive baseEnemyShip