#include "Projectile.h"


Projectile::Projectile(void)
{
	position = direction = startPosition = D3DXVECTOR3(0, 0, 0);
	mesh = NULL;
	texture = NULL;
	D3DXMatrixScaling(&scaleMat, 0.1f, 0.1f, 0.1f);
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 0, 0, 0);
	destroyObject = false;
}

Projectile::Projectile(D3DXVECTOR3 spawnPosition, D3DXVECTOR3 direction)
{
	position = startPosition = spawnPosition;
	this->direction = direction;
	D3DXMatrixScaling(&scaleMat, 0.1f, 0.1f, 0.1f);
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 0, 0, 0);
	destroyObject = false;

	Initialize();
}

Projectile::~Projectile(void)
{
	// Shutdown the variables
	Shutdown();
}

void Projectile::Initialize()
{
	// Load the mesh
	D3DXLoadMeshFromX(L"sphere.x", D3DXMESH_MANAGED, 
		Initializer::GetInstance()->GetDevice(), 
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
			D3DXCreateTextureFromFile(Initializer::GetInstance()->GetDevice(),
				filename, &texture[i]);
			delete[] ucString;
		}
	}

	// build bounding box for the mesh
	BYTE* vertices = NULL;
	mesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&vertices);
	D3DXComputeBoundingBox((D3DXVECTOR3*)vertices, mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()), &meshBox.minPt, &meshBox.maxPt);
	mesh->UnlockVertexBuffer();
}
	
void Projectile::Update(float dt)
{
	// Update the bullet's position
	position += direction * dt;

	// Check when the bullet has traveled a certain distance
	// either on the x-asix or y-axis
	if(position.x - startPosition.x >= 10.0f ||
		position.y - startPosition.y >= 10.0f)
	{
 		destroyObject = true;
	}

	// Set the translate matrix based on the player's current position
	D3DXMatrixTranslation(&translateMat, position.x, position.y, position.z);

	// Set the world matrix
	// Note: world = scale * rotate * translate
	worldMat = scaleMat * rotateMat* translateMat;

	// Set the collision box based on the player's world position
	//meshBox.xform(worldMat, meshBox);
}

void Projectile::Render(ID3DXEffect* shader)
{
	// WVPMat - World View Projection Matrix
	// WITMat - World Inverse Transverse Matrix
	D3DXMATRIX WVPMat, WITMat;

	// Set the WVPMat
	WVPMat = worldMat * Camera::GetInstance()->GetViewMat() * 
		Camera::GetInstance()->GetProjMat();

	// Set the WITMat
	D3DXMatrixInverse(&WITMat, 0, &worldMat);
	D3DXMatrixTranspose(&WITMat, &WITMat);

	// Set shader variables
	shader->SetMatrix("worldViewProjectionMatrix", &WVPMat);
	shader->SetMatrix("worldInverseTransposeMatrix", &WITMat);
	shader->SetMatrix("worldMatrix", &worldMat);
	shader->SetValue("eyePos", &Camera::GetInstance()->GetEyePos(), 
		sizeof(D3DXVECTOR3));

	// all of our draw calls go here
	shader->SetTechnique("tech0");
	UINT numPasses = 0;
	shader->Begin(&numPasses, 0);
	for(UINT i = 0; i < numPasses; i++)
	{
		shader->BeginPass(i);
		for(DWORD j = 0; j < numMaterials; j++)
		{
			shader->SetValue("ambientMaterial", &modelMaterial[j].Ambient, sizeof(D3DXCOLOR));
			shader->SetValue("diffuseMaterial", &modelMaterial[j].Diffuse, sizeof(D3DXCOLOR));
			shader->SetValue("specularMaterial", &modelMaterial[j].Specular, sizeof(D3DXCOLOR));
			shader->SetFloat("specularPower", modelMaterial[j].Power);
			shader->SetTexture("tex", texture[j]);
			shader->SetBool("usingTexture", true);
			shader->CommitChanges();
			mesh->DrawSubset(j);
		}
		shader->EndPass();
	}
	shader->End();
}

/// Release the object's variables
void Projectile::Shutdown()
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