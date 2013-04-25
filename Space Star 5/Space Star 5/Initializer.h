#pragma once

/*
The Initializer is to be used as a singleton class to store variables
and functions that will help initialize variables like meshes and textures.
*/

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using namespace std;

// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

class Initializer
{
private:
	static Initializer* instance;
	Initializer(void);

	IDirect3DDevice9*	m_pD3DDevice;	// Direct3D9 Device
	ID3DXFont*			gameFont;		// Direct3D font
	ID3DXSprite*		m_pD3DSprite;	// sprite variables

public:

	// mesh object
	struct MeshObject
	{
		ID3DXMesh*	mesh;
		LPDIRECT3DTEXTURE9* texture;
		DWORD numMaterials;
		D3DXMATERIAL* D3DXMaterial;
		D3DMATERIAL9* modelMaterial;

		void Init(LPCWSTR fileName)
		{
			ID3DXBuffer* materialBuff;

			// Load the mesh
			D3DXLoadMeshFromX(fileName, D3DXMESH_MANAGED, 
				Initializer::GetInstance()->GetDevice(), 
				NULL, &materialBuff, NULL, &numMaterials, &mesh);

			// Load the mesh's texture
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
					D3DXCreateTextureFromFile(Initializer::GetInstance()->GetDevice(),
						filename, &texture[i]);
					delete[] ucString;
				}
			}

			SAFE_RELEASE(materialBuff);
		}

		void Shutdown()
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
		}
	}playerMesh, projectileMesh, enemyMesh;

	~Initializer(void);

	// Singleton access methods
	static Initializer* GetInstance(void);
	static void DestroyInstance(void);

	void Initialize(HWND hWnd, IDirect3D9* m_pD3DObject, 
		D3DPRESENT_PARAMETERS D3Dpp);

	void Shutdown();

	IDirect3DDevice9* GetDevice() {return m_pD3DDevice;}
	ID3DXFont* GetFont() {return gameFont;}
	ID3DXSprite* GetSprite() {return m_pD3DSprite;}
	MeshObject GetPlayerMesh() {return playerMesh;}
	MeshObject GetProjectileMesh() {return projectileMesh;}
	MeshObject GetEnemyMesh() {return enemyMesh;}
};