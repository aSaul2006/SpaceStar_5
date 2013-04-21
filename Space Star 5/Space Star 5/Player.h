#pragma once

#include "InputManager.h"
#include "Camera.h"
#include "HUD.h"

// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

class Player
{
private:
	D3DXMATRIX scaleMat, rotateMat, translateMat, worldMat;
	D3DXVECTOR3 position;
	ID3DXMesh*	mesh;
	LPDIRECT3DTEXTURE9* texture;
	ID3DXBuffer* materialBuff;
	DWORD numMaterials;
	D3DXMATERIAL* D3DXMaterial;
	D3DMATERIAL9* modelMaterial;

	float rotateAngle;
	AABB meshBox;	// mesh's collision box

	// player HUD variable
	HUD playerHUD;

	// player health variables
	float currentHealth;
	float maxHealth;

	// player score variable
	int score;

	// player lives variable
	short lives;
public:
	Player(void);
	~Player(void);

	void Initialize();
	void Initialize2(LPCWSTR fileName);
	void Update(float dt);
	void Render(ID3DXEffect* shader);
	void Shutdown();

	// Accessors
	D3DXVECTOR3 GetPosition() {return position;}				// get position
	ID3DXMesh* GetMesh() {return mesh;}							// get mesh
	LPDIRECT3DTEXTURE9* GetTexture() {return texture;}			// get texture
	D3DXMATRIX GetWorldMatrix() {return worldMat;}				// get world matrix
	DWORD GetNumMaterials() {return numMaterials;}				// get number of materials
	D3DMATERIAL9* GetModelMaterial() {return modelMaterial;}	// get model material
	
	// get collision box
	AABB GetMeshBox()
	{
		AABB out;
		meshBox.xform(worldMat, out);
		return out;
	}

	// Dummy ship functions for collision testing
	// Delete after testing collision
	void SetPosition(D3DXVECTOR3 pos) {position = pos;}

	// Modify player's current health
	void IncrCurrHlth(float incrNum) {currentHealth += incrNum;}
	void DecrCurrHlth(float decrNum) {currentHealth -= decrNum;}
	void SetCurrHlth(float setHlth)	{currentHealth = setHlth;}

	// Modify player's max health
	void IncrMaxHlth(float incrNum) {maxHealth += incrNum;}
	void DecrMaxHlth(float decrNum) {maxHealth -= decrNum;}
	void SetMaxHlth(float setHlth) {maxHealth = setHlth;}

	// Access player's health
	float GetCurrHlth(void) {return currentHealth;}
	float GetMaxHlth(void)	{return maxHealth;}

	short GetNumLives(void) {return lives;}
};

