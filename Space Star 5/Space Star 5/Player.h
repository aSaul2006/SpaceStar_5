#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "InputManager.h"
#include "Camera.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

class Player
{
private:
	D3DXMATRIX scaleMat, rotateMat, translateMat;
	D3DXVECTOR3 position;
	ID3DXMesh*	mesh;
	LPDIRECT3DTEXTURE9* texture;
	ID3DXBuffer* materialBuff;
	DWORD numMaterials;
	D3DXMATERIAL* D3DXMaterial;
	D3DMATERIAL9* modelMaterial;

	float rotateAngle;
	AABB meshBox;	// mesh's collision box
public:
	Player(void);
	~Player(void);

	void Initialize(IDirect3DDevice9* m_pD3DDevice);
	void Initialize2(IDirect3DDevice9* m_pD3DDevice,  LPCWSTR fileName);
	void Update(float dt);
	void Render(ID3DXEffect* shader);
	void Shutdown();

	// Accessors
	D3DXVECTOR3 GetPosition() {return position;}
	ID3DXMesh* GetMesh() {return mesh;}
	LPDIRECT3DTEXTURE9* GetTexture() {return texture;}
	D3DXMATRIX GetScaleMat() {return scaleMat;}
	D3DXMATRIX GetRotateMat() {return rotateMat;}
	D3DXMATRIX GetTranslateMat() {return translateMat;}
	DWORD GetNumMaterials() {return numMaterials;}
	D3DMATERIAL9* GetModelMaterial() {return modelMaterial;}
};

