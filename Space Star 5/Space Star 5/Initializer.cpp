#include "Initializer.h"

Initializer* Initializer::instance;

Initializer::Initializer(void)
{
	m_pD3DDevice = NULL;
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
}

void Initializer::Shutdown()
{
	SAFE_RELEASE(m_pD3DDevice);
}