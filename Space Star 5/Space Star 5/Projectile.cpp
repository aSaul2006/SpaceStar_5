#include "Projectile.h"


Projectile::Projectile(void)
{
	position = direction = startPosition = D3DXVECTOR3(0, 0, 0);
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
	Shutdown();
}

void Projectile::Initialize()
{
	// build bounding box for the mesh
	BYTE* vertices = NULL;
	Initializer::GetInstance()->GetProjectileMesh().mesh->LockVertexBuffer(
		D3DLOCK_READONLY, (LPVOID*)&vertices);

	D3DXComputeBoundingBox((D3DXVECTOR3*)vertices, 
		Initializer::GetInstance()->GetProjectileMesh().mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(
		Initializer::GetInstance()->GetProjectileMesh().mesh->GetFVF()), 
		&meshBox.minPt, &meshBox.maxPt);

	Initializer::GetInstance()->GetProjectileMesh().mesh->UnlockVertexBuffer();
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
		for(DWORD j = 0; j < 
			Initializer::GetInstance()->GetProjectileMesh().numMaterials; j++)
		{
			shader->SetValue("ambientMaterial",
				&Initializer::GetInstance()->GetProjectileMesh().modelMaterial[j].Ambient, sizeof(D3DXCOLOR));
			shader->SetValue("diffuseMaterial", 
				&Initializer::GetInstance()->GetProjectileMesh().modelMaterial[j].Diffuse, sizeof(D3DXCOLOR));
			shader->SetValue("specularMaterial", 
				&Initializer::GetInstance()->GetProjectileMesh().modelMaterial[j].Specular, sizeof(D3DXCOLOR));
			shader->SetFloat("specularPower", 
				Initializer::GetInstance()->GetProjectileMesh().modelMaterial[j].Power);
			shader->SetTexture("tex", 
				Initializer::GetInstance()->GetProjectileMesh().texture[j]);
			shader->SetBool("usingTexture", true);
			shader->CommitChanges();
			Initializer::GetInstance()->GetProjectileMesh().mesh->DrawSubset(j);
		}
		shader->EndPass();
	}
	shader->End();
}

/// Release the object's variables
void Projectile::Shutdown()
{
}