#pragma once

/*
	The Initializer is to be used as a singleton class to store variables
	and functions that will help initialize variables like meshes and textures.
*/

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

class Initializer
{
private:
	static Initializer* instance;
	Initializer(void);

	IDirect3DDevice9*	m_pD3DDevice;	// Direct3D9 Device
	ID3DXFont* gameFont;				// Direct3D font

public:
	~Initializer(void);

	// Singleton access methods
	static Initializer* GetInstance(void);
	static void DestroyInstance(void);

	void Initialize(HWND hWnd, IDirect3D9* m_pD3DObject, 
		D3DPRESENT_PARAMETERS D3Dpp);
	void Shutdown();

	IDirect3DDevice9* GetDevice() {return m_pD3DDevice;}
	ID3DXFont* GetFont() {return gameFont;}
};