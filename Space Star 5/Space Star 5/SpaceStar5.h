#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
#include <list>
#include "AudioManager.h"
#include "InputManager.h"
#include "Camera.h"
#include "Player.h"
#include "EnemyShip.h"
#include "Skybox.h"
#include "Projectile.h"
#include "EnemyShip.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using namespace std;

// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

class SpaceStar5
{
private:
	D3DPRESENT_PARAMETERS D3Dpp;

	/// D3D9 Variables
	IDirect3D9*			m_pD3DObject;	// Direct3D9 Object
	IDirect3DDevice9*	m_pD3DDevice;	// Direct3D9 Device
	D3DCAPS9			m_D3DCaps;		// find the capabilities of graphics card

	// applications variables
	bool				m_bVSync;

	//sprite variables
	ID3DXSprite*		m_pD3DSprite;

	//////////////////////////////////////////////////////////////////////////////
	///// Test Variables - Used for testing
	//////////////////////////////////////////////////////////////////////////////
	ID3DXEffect* shader;
	ID3DXBuffer* errorCheck;

	Player player;
	baseEnemyShip baseEnemy;

	Skybox skybox;

	list<Projectile*> pList;
	Enemy enemy;
public:
	SpaceStar5(void);
	~SpaceStar5(void);

	void Init(HWND hWnd, HINSTANCE hInstance, bool bWindowed);
	void Update(float dt);
	void Render();
	void Shutdown();

	void RenderSkybox();
};

