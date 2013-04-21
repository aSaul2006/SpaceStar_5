#include "EnemyShip.h"
#include "Camera.h"
#include <math.h>
#include <list>

float radius = 7.5;
float cos_y = 0.0;
float start = 0.0;
float angle = 1.0;
float x2 = 0.0,y2 = 0.0,fx = 0.0,fy = 0.0, cacheCosY = 0.0 ;
int cache = 0;


//Base enemy class.  Any enemy ship class made will derive directly from this class and override some behavior.
baseEnemyShip::~baseEnemyShip()
{
	Shutdown();
}

baseEnemyShip::baseEnemyShip()
{

}

void baseEnemyShip::initializeEnemyShip(LPCWSTR fileName)
{
	// Load the mesh
	D3DXLoadMeshFromX(fileName, D3DXMESH_MANAGED, 
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

/// Enemy class

Enemy::Enemy()
{
	// Initialize variables to 0 or NULL
	D3DXMatrixScaling(&scaleMat, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 0, 0, 0);
	D3DXMatrixTranslation(&translateMat, 0, 0, 0);
	m_position = m_velocity = D3DXVECTOR3(0, 0, 0);
	mesh = NULL;
	texture = NULL;
	track = health = maxHealth = 0;	// change later if needed
	isHidden = moveDir = isHealthZero = destroyObject = false;	// change later if needed

}


Enemy::~Enemy()
{
}

void Enemy::Render(ID3DXEffect* shader)
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


void Enemy::fireWeapon(int fireRate, Player* player)
{
	enemyBullet.push_front(new Projectile(m_position, D3DXVECTOR3(-10.0f,0.0,0.0)));
}

void Enemy::renderBullet(ID3DXEffect* shader)
{
	// Render projectiles
	for each(Projectile* projectile in enemyBullet)
	{
		projectile->Render(shader);
	}
}

void Enemy::SetEnemyAttrib(int shipHealth,float speed,float rate, D3DXVECTOR3 pos, AttackType at)
{
	m_position = pos;
	health = shipHealth;
	m_fireRate = rate;
	m_speed = speed;
	m_attackType = at;
}

void Enemy::update(float dt, Player * player)
{
	D3DXVECTOR3 playerPos = player->GetPosition();

	// player's rotation speed
	float rotateSpeed = 0.2f;

	// check if the player needs to rotate
	bool rotate = false;

	switch(m_attackType)
	{
	case ATTACK1:
		m_position.x -= m_speed * dt;
		if(fmod(dt*(float)track,300) == 0 && isHidden == false)
			fireWeapon(2,player);
		break;
	case ATTACK2:
		m_position.x -= m_speed * dt;
		m_position.y += 1.0 * dt;
		if(fmod(dt*(float)track,300) == 0 && isHidden == false)
			fireWeapon(2,player);
		break;
	case ATTACK3:
		m_position.x -= m_speed * dt;

		if(playerPos.y < m_position.y)
		{
			m_position.y -= m_speed * dt;
		}
		if(playerPos.y > m_position.y)
		{
			m_position.y += m_speed * dt;
		}
		if(fmod(dt*(float)track,100) == 0 && isHidden == false)
			fireWeapon(2,player);
		break;
	case ATTACK4:
		start += 1.0;
		if(start > 360)
		{
			start = 0;
		}
		if(angle > 720)
			angle = 0.0;

		float rad_angle = (angle * 3.14)/180;
		m_position.x -= m_speed * dt;
		//y2 = radius * sin((double)rad_angle);
		m_position.y = 2.0 * sin((double)(-rad_angle));
		angle += 1.0;

		if(fmod(dt*(float)track,150) == 0 && isHidden == false)
			fireWeapon(2,player);
		break;
	/*case AVOID1:
		break;
	case AVOID2:
		break;
	case DEFAULT:
		break;*/
	}	

	for each (Projectile* projectile in enemyBullet)
	{
		projectile->Update(dt);

		// testing collision
		if(projectile->GetMeshBox().Intersects(player->GetMeshBox()))
		{
			projectile->Destroy();
			player->DecrCurrHlth(20.0f);
		}

	}

	for(std::list<Projectile*>::const_iterator i = enemyBullet.begin(), end = enemyBullet.end(); i != end;)
	{	
		if((*i)->CheckObject())
		{
			delete (*i);
			i = enemyBullet.erase(i);
		}
		else
			i++;		
	}

	
	D3DXMatrixTranslation(&translateMat, m_position.x, m_position.y, m_position.z);


	track ++;
	if(track == 600)
		track = 0;

}

void Enemy::calculateDamage(int power)
{
	health -= power;
}

void Enemy::destroyShip()
{
	destroyObject = true;
}


//void Enemy::loadEnemies(std::list<Enemy*> pEnemies,int num)
//{
//	for(int i = 0; i < num; i ++)
//	{
//		pEnemies.push_front(new Enemy());
//	}
//
//}