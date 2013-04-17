#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "Camera.h"
#include "InputManager.h"
#include "Initializer.h"

// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

struct TVertex
{
	float x, y, z;
	float tu, tv;
};

#define FVF_FLAGS D3DFVF_XYZ | D3DFVF_TEX1

class Skybox
{
private:
	IDirect3DVertexBuffer9* vertexBuffer;
	IDirect3DTexture9* skyTextures[6];

	D3DXMATRIX viewMat;
	float rotateY;
public:
	Skybox(void);
	~Skybox(void);

	void BuildSkybox(float screenWidth, float screenHeight);
	void Update(float dt);
	void Render();
	void Shutdown();
};

