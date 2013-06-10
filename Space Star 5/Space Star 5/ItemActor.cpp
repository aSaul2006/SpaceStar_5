//ItemActor Class
// This base class will be inherited by other classes implementing
// Items dropped by enemies
#include "ItemActor.h"

ItemActor::ItemActor(void)
{
	
}
ItemActor::~ItemActor()
{
	Shutdown();
}

void ItemActor::Shutdown()
{
	// uncomment when sound gets added
	/*if(itemSFX!=NULL)
	{
		itemSFX->release();
		itemSFX = NULL;
	}*/
}

void ItemActor::DropItem(std::list<ItemActor*> &items,D3DXVECTOR3 pos)
{
	int pickAnItem = (rand() % 4) + 1;

	ItemActor* item;
		switch(pickAnItem)
		{
		case 1:
			item = new HealthItemActor(pos);
			items.push_front(item);
			break;
		case 2:
			item = new Missile1ItemActor(pos);
			items.push_front(item);
			break;
		case 3:
			item = new HealthItemActor(pos);
			items.push_front(item);
			break;
		case 4:
			item = new Missile1ItemActor(pos);
			items.push_front(item);
			break;
		}

		
}

//-----------------------------------------------------------------------------
//************************* Health Pick up class ******************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------
HealthItemActor::HealthItemActor(D3DXVECTOR3 pos)
{
	// Initialize variables to 0 or NULL
	D3DXMatrixScaling(&scaleMat, 0.1f, 0.1f, 0.1f);
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 0, 0, 0);
	D3DXMatrixTranslation(&translateMat, 0, 0, 0);
	m_position = pos;
	m_rotateAngle = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_speed = 0.5;
	this->Initialize();
	typeOfItemDropped = HEALTH;
	destroyObject = false;
}

HealthItemActor::~HealthItemActor(void)
{
}

void HealthItemActor::Update(float dt, baseEnemyShip* enemy)
{
	m_position.x -= m_speed * dt;

	//m_rotateAngle.y += .05;

	//if(m_rotateAngle.y == 360)
	//	m_rotateAngle.y = 0;

	D3DXMatrixRotationYawPitchRoll(&rotateMat, 
		D3DXToRadian(m_rotateAngle.y), 0, 0);

	D3DXMatrixTranslation(&translateMat, m_position.x, m_position.y, m_position.z);

	worldMat = scaleMat * rotateMat* translateMat;
}

void HealthItemActor::Render(ID3DXEffect* shader)
{
	D3DXMATRIX WVPMat, WITMat;

	// Scale, Rotate, and translate the model's worldMat
	worldMat = scaleMat * rotateMat * translateMat;

	WVPMat = worldMat * Camera::GetInstance()->GetViewMat() * 
		Camera::GetInstance()->GetProjMat();

	D3DXMatrixInverse(&WITMat, 0, &worldMat);
	D3DXMatrixTranspose(&WITMat, &WITMat);

	//shader->SetTexture("tex", player.GetTexture());
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
			Initializer::GetInstance()->GetHealthPickupMesh().numMaterials; j++)
		{
			shader->SetValue("ambientMaterial", 
				&Initializer::GetInstance()->GetHealthPickupMesh().modelMaterial[j].Ambient, sizeof(D3DXCOLOR));
			shader->SetValue("diffuseMaterial", 
				&Initializer::GetInstance()->GetHealthPickupMesh().modelMaterial[j].Diffuse, sizeof(D3DXCOLOR));
			shader->SetValue("specularMaterial", 
				&Initializer::GetInstance()->GetHealthPickupMesh().modelMaterial[j].Specular, sizeof(D3DXCOLOR));
			shader->SetFloat("specularPower", 
				Initializer::GetInstance()->GetHealthPickupMesh().modelMaterial[j].Power);
			shader->SetTexture("tex", 
				Initializer::GetInstance()->GetHealthPickupMesh().texture[j]);
			shader->SetBool("usingTexture", true);
			shader->CommitChanges();
			Initializer::GetInstance()->GetHealthPickupMesh().mesh->DrawSubset(j);
		}
		shader->EndPass();
	}
	shader->End();
}

void HealthItemActor::Initialize(void)
{
	// build bounding box for the mesh
	BYTE* vertices = NULL;
	Initializer::GetInstance()->GetHealthPickupMesh().mesh->LockVertexBuffer(
		D3DLOCK_READONLY, (LPVOID*)&vertices);

	D3DXComputeBoundingBox((D3DXVECTOR3*)vertices, 
		Initializer::GetInstance()->GetHealthPickupMesh().mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(
		Initializer::GetInstance()->GetHealthPickupMesh().mesh->GetFVF()), 
		&meshBox.minPt, &meshBox.maxPt);

	Initializer::GetInstance()->GetHealthPickupMesh().mesh->UnlockVertexBuffer();

	//Add a sound later when I come up with something
	//AudioManager::GetInstance()->GetSystem()->createSound("laser3.wav",FMOD_DEFAULT,0, &itemSFX);
}


//-----------------------------------------------------------------------------
//************************* Missile 1 Pick up class ***************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

Missile1ItemActor::Missile1ItemActor(D3DXVECTOR3 pos)
{
	// Initialize variables to 0 or NULL
	D3DXMatrixScaling(&scaleMat, 0.1f, 0.1f, 0.1f);
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 0, 0, 0);
	D3DXMatrixTranslation(&translateMat, 0, 0, 0);
	m_position = pos;
	m_rotateAngle = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_speed = 0.5;
	this->Initialize();
	typeOfItemDropped = MISSILES;
	destroyObject = false;
}

Missile1ItemActor::~Missile1ItemActor(void)
{
}

void Missile1ItemActor::Update(float dt, baseEnemyShip* enemy)
{
	m_position.x -= m_speed * dt;

	m_rotateAngle.y += .05f;
	if(m_rotateAngle.y == 360)
		m_rotateAngle.y = 0;

	D3DXMatrixRotationYawPitchRoll(&rotateMat, 
		D3DXToRadian(m_rotateAngle.y), 0,0);

	D3DXMatrixTranslation(&translateMat, m_position.x, m_position.y, m_position.z);

	worldMat = scaleMat * rotateMat* translateMat;
}

void Missile1ItemActor::Render(ID3DXEffect* shader)
{
	D3DXMATRIX WVPMat, WITMat;

	// Scale, Rotate, and translate the model's worldMat
	worldMat = scaleMat * rotateMat * translateMat;

	WVPMat = worldMat * Camera::GetInstance()->GetViewMat() * 
		Camera::GetInstance()->GetProjMat();

	D3DXMatrixInverse(&WITMat, 0, &worldMat);
	D3DXMatrixTranspose(&WITMat, &WITMat);

	//shader->SetTexture("tex", player.GetTexture());
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
			Initializer::GetInstance()->GetMissilePickupMesh().numMaterials; j++)
		{
			shader->SetValue("ambientMaterial", 
				&Initializer::GetInstance()->GetMissilePickupMesh().modelMaterial[j].Ambient, sizeof(D3DXCOLOR));
			shader->SetValue("diffuseMaterial", 
				&Initializer::GetInstance()->GetMissilePickupMesh().modelMaterial[j].Diffuse, sizeof(D3DXCOLOR));
			shader->SetValue("specularMaterial", 
				&Initializer::GetInstance()->GetMissilePickupMesh().modelMaterial[j].Specular, sizeof(D3DXCOLOR));
			shader->SetFloat("specularPower", 
				Initializer::GetInstance()->GetMissilePickupMesh().modelMaterial[j].Power);
			shader->SetTexture("tex", 
				Initializer::GetInstance()->GetMissilePickupMesh().texture[j]);
			shader->SetBool("usingTexture", true);
			shader->CommitChanges();
			Initializer::GetInstance()->GetMissilePickupMesh().mesh->DrawSubset(j);
		}
		shader->EndPass();
	}
	shader->End();
}

void Missile1ItemActor::Initialize(void)
{
	// build bounding box for the mesh
	BYTE* vertices = NULL;
	Initializer::GetInstance()->GetMissilePickupMesh().mesh->LockVertexBuffer(
		D3DLOCK_READONLY, (LPVOID*)&vertices);

	D3DXComputeBoundingBox((D3DXVECTOR3*)vertices, 
		Initializer::GetInstance()->GetMissilePickupMesh().mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(
		Initializer::GetInstance()->GetMissilePickupMesh().mesh->GetFVF()), 
		&meshBox.minPt, &meshBox.maxPt);

	Initializer::GetInstance()->GetMissilePickupMesh().mesh->UnlockVertexBuffer();

	//Add a sound later when I come up with something
	//AudioManager::GetInstance()->GetSystem()->createSound("laser3.wav",FMOD_DEFAULT,0, &itemSFX);
}
