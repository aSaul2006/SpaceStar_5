#include "EnemyShip.h"
#include "Camera.h"
#include <math.h>
#include <list>
#include <random>
#include <time.h>
#include "Misc\CrudeTimer.h"

float radius = 10.0;
float start = 0.0;

std::default_random_engine gen;
std::uniform_int_distribution<int> attackSwitch(0,3);

//Base enemy class.  Any enemy ship class made will derive directly from this class and override some behavior.
baseEnemyShip::~baseEnemyShip()
{
	Shutdown();
}

baseEnemyShip::baseEnemyShip()
{

}

void baseEnemyShip::initializeEnemyShip()
{
	// build bounding box for the mesh
	BYTE* vertices = NULL;
	Initializer::GetInstance()->GetViperMesh().mesh->LockVertexBuffer(
		D3DLOCK_READONLY, (LPVOID*)&vertices);

	D3DXComputeBoundingBox((D3DXVECTOR3*)vertices, 
		Initializer::GetInstance()->GetViperMesh().mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(
		Initializer::GetInstance()->GetViperMesh().mesh->GetFVF()), 
		&meshBox.minPt, &meshBox.maxPt);

	Initializer::GetInstance()->GetViperMesh().mesh->UnlockVertexBuffer();

	AudioManager::GetInstance()->GetSystem()->createSound("laser3.wav",FMOD_DEFAULT,0, &enemySFX);
}



void baseEnemyShip::Shutdown()
{
	enemySFX->release();
}

/// Enemy class

Viper::Viper()
{
	// Initialize variables to 0 or NULL
	D3DXMatrixScaling(&scaleMat, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 0, 0, 0);
	D3DXMatrixTranslation(&translateMat, 0, 0, 0);
	m_position = m_velocity = D3DXVECTOR3(0, 0, 0);
	angle = 0.0;
	track = health = maxHealth = 0;	// change later if needed
	hasSpawned = isHidden = moveDir = isHealthZero = destroyObject = false;	// change later if needed
	spawnTime = CrudeTimer::Instance()->GetTickCount();
}


Viper::~Viper()
{

}

void Viper::Render(ID3DXEffect* shader)
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
			Initializer::GetInstance()->GetViperMesh().numMaterials; j++)
		{
			shader->SetValue("ambientMaterial", 
				&Initializer::GetInstance()->GetViperMesh().modelMaterial[j].Ambient, sizeof(D3DXCOLOR));
			shader->SetValue("diffuseMaterial", 
				&Initializer::GetInstance()->GetViperMesh().modelMaterial[j].Diffuse, sizeof(D3DXCOLOR));
			shader->SetValue("specularMaterial", 
				&Initializer::GetInstance()->GetViperMesh().modelMaterial[j].Specular, sizeof(D3DXCOLOR));
			shader->SetFloat("specularPower", 
				Initializer::GetInstance()->GetViperMesh().modelMaterial[j].Power);
			shader->SetTexture("tex", 
				Initializer::GetInstance()->GetViperMesh().texture[j]);
			shader->SetBool("usingTexture", true);
			shader->CommitChanges();
			Initializer::GetInstance()->GetViperMesh().mesh->DrawSubset(j);
		}
		shader->EndPass();
	}
	shader->End();
}


void Viper::fireWeapon(int fireRate, Player* player)
{
	enemyBullet.push_front(new Projectile(m_position, D3DXVECTOR3(-10.0f,0.0,0.0)));
	AudioManager::GetInstance()->PlaySFX(enemySFX);
}

void Viper::renderBullet(ID3DXEffect* shader)
{
	// Render projectiles
	for each(Projectile* projectile in enemyBullet)
	{
		if(projectile->GetPosition().x > -12 &&
			projectile->GetPosition().x < 12)
		{
			projectile->Render(shader);
		}
	}
}

void Viper::SetEnemyAttrib(int shipHealth,float speed,float rate, D3DXVECTOR3 pos)
{
	srand(time(NULL));
	m_position = pos;
	health = shipHealth;
	m_fireRate = rate;
	m_speed = speed;
	int attack = rand() % 6;
	switch(attack)
	{
	case 0:
		m_attackType = ATTACK1;
		break;
	case 1:
		m_attackType = ATTACK2;
		break;
	case 2:
		m_attackType = ATTACK3;
		break;
	case 3:
		m_attackType = ATTACK4;
		break;
	case 4:
		m_attackType = AVOID1;
		break;
	case 5:
		m_attackType = AVOID2;
		break;
	}
}

void Viper::SetEnemyAttrib2(int shipHealth,float speed,AttackType at, D3DXVECTOR3 pos)
{
	m_position = pos;
	health = shipHealth;
	m_attackType = at;
	m_speed = speed;
}

void Viper::update(float dt, Player * player)
{
	D3DXVECTOR3 playerPos = player->GetPosition();

	float rad_angle = 0.0f;
	// player's rotation speed
	float rotateSpeed = 0.2f;

	// check if the player needs to rotate
	bool rotate = false;

	switch(m_attackType)
	{
	case ATTACK1:
		m_position.x -= m_speed * dt;
		m_rotateAngle = 0;

		break;
	case ATTACK2:
		m_position.x -= m_speed * dt;
		if(m_position.x < 4)
		{
			m_position.y += m_speed * dt;

		}
		if(m_position.y > -7)
		{
			m_rotateAngle += rotateSpeed;
			rotate = true;
		}
		else
			rotate = false;	

		break;
	case ATTACK3:
		m_position.x -= m_speed * dt;

		if(playerPos.y < m_position.y)
		{
			m_position.y -= m_speed * dt;
			m_rotateAngle -= rotateSpeed;
			rotate = true;
		}

		if(playerPos.y > m_position.y)
		{
			m_position.y += m_speed * dt;
			m_rotateAngle += rotateSpeed;
			rotate = true;
		}

		break;
	case ATTACK4:
		if(angle == 360)
			angle = 0.0;

		rad_angle = (angle * 3.14)/180;
		m_position.x -= m_speed * dt;
		m_position.y = 2.0 * sin((double)(-rad_angle));
		m_rotateAngle = 0;

		angle += 1.0;
	
		break;
	case ATTACK5:
		m_position.x -= m_speed * dt;
		if(m_position.x < 4)
		{
			m_position.y -= m_speed * dt;

		}
		if(m_position.y < 5)
		{
			m_rotateAngle -= rotateSpeed;
			rotate = true;
		}
		else 
			rotate = false;
		

		break;
	case AVOID1:
		m_position.x -= m_speed * dt;
		m_position.y += m_speed * dt;

		if(m_position.y > -7)
		{
			m_rotateAngle += rotateSpeed;
			rotate = true;
		}

		if(m_position.y >= 5)
			m_attackType = AVOID2;  
		break;
	case AVOID2:
		m_position.x -= m_speed * dt;
		m_position.y -= m_speed * dt;

		if(m_position.y < 5)
		{
			m_rotateAngle -= rotateSpeed;
			rotate = true;
		}

		if(m_position.y <= -7)
			m_attackType = AVOID1;
		break;
	}

	if(!isHidden)
	{
		if((int)playerPos.y == (int)m_position.y)
		{
			if(fmod((float)track,237) == 0)
				fireWeapon(2,player);
		}
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


	if(rotate)
	{
		if(m_rotateAngle < -45.0f)
			m_rotateAngle = -45.0f;
		if(m_rotateAngle > 45.0f)
			m_rotateAngle = 45.0f;
	}
	else
	{
		if(m_rotateAngle < 0)
			m_rotateAngle += rotateSpeed;
		if(m_rotateAngle > 0)
			m_rotateAngle -= rotateSpeed;
	}

	D3DXMatrixRotationYawPitchRoll(&rotateMat, 
		D3DXToRadian(0.0f), D3DXToRadian(m_rotateAngle), 0);
	
	D3DXMatrixTranslation(&translateMat, m_position.x, m_position.y, m_position.z);

	worldMat = scaleMat * rotateMat* translateMat;

	track ++;
	if(track >= 600)
		track = 0;

}

void Viper::calculateDamage(int power)
{
	health -= power;
}

void Viper::destroyShip()
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