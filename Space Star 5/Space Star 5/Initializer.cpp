#include "Initializer.h"

Initializer* Initializer::instance;

Initializer::Initializer(void)
{
	m_pD3DDevice = NULL;
	gameFont = NULL;
}

Initializer::~Initializer(void)
{
	Shutdown();
}

Initializer* Initializer::GetInstance()
{
	if(instance == 0)
		instance = new Initializer();
	return instance;
}

void Initializer::DestroyInstance()
{
	if(instance)
	{
		delete instance;
		instance = NULL;
	}
}

void Initializer::Initialize(HWND hWnd, IDirect3D9* m_pD3DObject, 
		D3DPRESENT_PARAMETERS D3Dpp)
{
	// Create the Direct3D device using the presentation parameters above
	m_pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,		// Primary Display Device
		D3DDEVTYPE_HAL,			// Device type to use, use HAL
		hWnd,					// handle to the focus window
		D3DCREATE_HARDWARE_VERTEXPROCESSING,	// hardware behavior flags
		&D3Dpp,					// presentation parameters
		&m_pD3DDevice);			// return created D3DDevice

	// Create Font COM object
	AddFontResourceEx(L"Spaceracer.ttf", FR_PRIVATE, 0);
	D3DXCreateFont(m_pD3DDevice, 30, 0, FW_BOLD, 0, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Spaceracer"),
		&gameFont);

	// Create your sprite COM object
	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);
}

void Initializer::Shutdown()
{
	SAFE_RELEASE(m_pD3DSprite);
	SAFE_RELEASE(gameFont);
	SAFE_RELEASE(m_pD3DDevice);
}