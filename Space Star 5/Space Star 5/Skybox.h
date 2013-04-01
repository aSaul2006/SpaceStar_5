#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "Camera.h"
#include "InputManager.h"

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
public:
	Skybox(void);
	~Skybox(void);

	void BuildSkybox(IDirect3DDevice9* m_pD3DDevice, float screenWidth,
		float screenHeight);
	void Render(IDirect3DDevice9* m_pD3DDevice);
	void Shutdown();
};

