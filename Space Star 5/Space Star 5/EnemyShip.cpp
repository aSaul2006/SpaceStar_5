#include "EnemyShip.h"


baseEnemyShip::~baseEnemyShip()
{
	Shutdown();
}

void baseEnemyShip::initializeEnemyShip(IDirect3DDevice9* m_pD3DDevice, LPCWSTR fileName)
{
	// Load the mesh
	D3DXLoadMeshFromX(fileName, D3DXMESH_MANAGED, m_pD3DDevice, 
		NULL, &materialBuff, NULL, &numMaterials, &mesh);

	D3DXMaterial = (D3DXMATERIAL*) materialBuff->GetBufferPointer();
	modelMaterial = new D3DMATERIAL9[numMaterials];
	texture = new LPDIRECT3DTEXTURE9[numMaterials];

	for(DWORD i = 0; i < numMaterials; i++)
	{
		modelMaterial[i] = D3DXMaterial[i].MatD3D;
		modelMaterial[i].Ambient.r = 0.1f;
		modelMaterial[i].Ambient.g = 0.1f;
		modelMaterial[i].Ambient.b = 0.1f;
		modelMaterial[i].Ambient.a = 1.0f;

		texture[i] = NULL;
		if(D3DXMaterial[i].pTextureFilename)
		{
			int len = 0;
			len = (int)strlen(D3DXMaterial[i].pTextureFilename) + 1;
			wchar_t *ucString = new wchar_t[len];
			mbstowcs(ucString, D3DXMaterial[i].pTextureFilename, len);
			LPCWSTR filename = (LPCWSTR)ucString;
			D3DXCreateTextureFromFile(m_pD3DDevice, filename, &texture[i]);
			delete[] ucString;
		}
	}
}

void baseEnemyShip::fireWeapon(int fireRate)
{

}

void baseEnemyShip::update(float dt)
{

}

void baseEnemyShip::calculateDamage()
{

}

void baseEnemyShip::setPositionAndVelocity(D3DXVECTOR3 pos, D3DXVECTOR3 vel)
{

}

void baseEnemyShip::destroyShip()
{

}

void baseEnemyShip::Shutdown()
{
	SAFE_RELEASE(mesh);
	if(texture)
	{
		for(DWORD i = 0; i < numMaterials; i++)
		{
			SAFE_RELEASE(texture[i]);
		}
		delete[] texture;
	}
	SAFE_RELEASE(materialBuff);
}