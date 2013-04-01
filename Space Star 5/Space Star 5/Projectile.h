#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "Camera.h"

// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

class Projectile
{
private:
	D3DXMATRIX scaleMat, rotateMat, translateMat;
	D3DXVECTOR3 position, direction;
	ID3DXMesh*	mesh;
	LPDIRECT3DTEXTURE9* texture;
	ID3DXBuffer* materialBuff;
	DWORD numMaterials;
	D3DXMATERIAL* D3DXMaterial;
	D3DMATERIAL9* modelMaterial;

	AABB meshBox;
public:
	Projectile(void);
	// Spawn position is the position of the object firing the projectile
	// Direction is the direction the projectile will be traveling
	Projectile(D3DXVECTOR3 spawnPosition, D3DXVECTOR3 direction,
		IDirect3DDevice9* m_pD3DDevice);
	~Projectile(void);

	void Initialize(IDirect3DDevice9* m_pD3DDevice);
	void Update(float dt);
	void Render(ID3DXEffect* shader);
	void Shutdown();
};

