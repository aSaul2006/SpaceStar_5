#include "GameScreen.h"
#include <random>
#include <time.h>
#include "AttackWaves.h"
#include "GameOverScreen.h"

ViperWave1 * wave;
ViperWave2 * wave2;
ViperWave3 * wave3;
ViperWave4 * wave4;
ViperWave5 * wave5;

default_random_engine generator;
uniform_int_distribution<int> randSpeed(4,6);
uniform_int_distribution<float> randYpos(-7,5);

GameScreen::GameScreen(void)
{
	type = GameType;
	Initialize();
	enemiesSpawned = 0;
	spawnTime = 0.0;
	enemyDestroyedCount = 0;
}

GameScreen::~GameScreen(void)
{
	Shutdown();
}

void GameScreen::Initialize(void)
{
	// Initialize the player
	player.Initialize();
	
	

	//////////////////////////////////////////////////////////////////////////////
	//// Camera Test - Initialize Object for testing
	//////////////////////////////////////////////////////////////////////////////
	D3DXCreateEffectFromFile(Initializer::GetInstance()->GetDevice(),
		L"lab5.fx", 0, 0, 0, 0, 
		&shader, &errorCheck);

	if(errorCheck)
	{
		MessageBoxA(0,(char*)errorCheck->GetBufferPointer(), 0, 0);
	}

	SAFE_RELEASE(errorCheck);

	// Create Skybox
	float screenWidth = 800;
	float screenHeight = 600;
	skybox.BuildSkybox((float)screenWidth, (float)screenHeight);

	// Initialize SFX
	AudioManager::GetInstance()->GetSystem()->createSound("8bitLaser1.wav", 
		FMOD_DEFAULT, 0, &projSFX);
	wave = new ViperWave1();
	wave2 = new ViperWave2();
	wave3 = new ViperWave3();
	wave4 = new ViperWave4();
	wave5 = new ViperWave5();
	wave3->AttackPattern(pEnemies);
	
	// initialize particle system
	D3DXMATRIX psysWorld;
	D3DXMatrixTranslation(&psysWorld, 0.0f, 0.0f, 0.0f);
	AABB psysBox;
	psysBox.minPt = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	psysBox.maxPt = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	PSys = new FireRing("firering.fx", "FireRingTech", "torch.dds",
		D3DXVECTOR3(0.0f, 0.9f, 0.0f), psysBox, 100, 0.0025f);
	//PSys->SetWorldMat(psysWorld);
	psysTime = 0;
	
}

void GameScreen::Update(GameState& gameState, float dt)
{
	srand(time(NULL));
	Camera::GetInstance()->Update(dt);
	player.Update(dt);
	skybox.Update(dt);
	
	PSys->Update(dt);
	psysTime += dt;

	if(psysTime >= 1.5f)
	{
		PSys->play = false;
		psysTime = 0;
		PSys->ResetTime();
	}
	

	//if(enemiesSpawned <= 5 && (CrudeTimer::Instance()->GetTickCount() - spawnTime) >= 2)
	//{
	//	pEnemies.push_front(new Enemy());
	//	spawnTime = CrudeTimer::Instance()->GetTickCount();
	//	enemiesSpawned ++;
	//}
	if(pEnemies.empty())
	{
		int pickAWave = rand() % 5;
		switch(pickAWave)
		{
		case 0:
			wave->AttackPattern(pEnemies);
			break;
		case 1:
			wave2->AttackPattern(pEnemies);
			break;
		case 2:
			wave3->AttackPattern(pEnemies);
			break;
		//case 3:
		//	wave4->AttackPattern(pEnemies);
		//	break;
		//case 4:
		//	wave5->AttackPattern(pEnemies);
		//	break;
		}
	}

	for each(baseEnemyShip* enemy in pEnemies)
	{
		//if(!enemy->hasSpawned)
		//{
		//	int enemySpeed = (rand()%(6-3))+3;;
		//	float enemyYpos =(rand()%(5-(-7)))+(-7);

		//	enemy->initializeEnemyShip();
		//	enemy->SetEnemyAttrib(100,enemySpeed,2.0f,D3DXVECTOR3(8.0f,enemyYpos,0.0f));
		//	enemy->hasSpawned = true;
		//}

		enemy->update(dt,&player);

		//check for enemies exiting the viewable screen
		if(!Camera::GetInstance()->IsVisible(enemy->GetMeshBox()))
		{
			if(enemy->getPosition().x < Camera::GetInstance()->GetEyePos().x)
			{
				enemy->destroyShip();
				enemiesSpawned --;
			}
			else if(enemy->getPosition().x > 12)
			{
				enemy->hideShip(true);
				
			}
			else if(enemy->getPosition().x < 12 && enemy->getPosition().x > 0)
				enemy->hideShip(false);
		}

		
	}
	
	//zoom in and out for debugging purposes ehh
	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_0))
	{
		Camera::GetInstance()->IncreaseEyePos(1);
	}

	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_9))
	{
		Camera::GetInstance()->DecreaseEyePos(1);
	}
	//enemy.update(dt,&player);

	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_SPACE))
	{
		pList.push_front(new Projectile(player.GetPosition(), D3DXVECTOR3(20.0f, 0, 0)));
		AudioManager::GetInstance()->PlaySFX(projSFX);
	}

	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_ESCAPE))
	{
		gameState = TitleMenu;
	}

	// update the projectiles in pList
	for each(Projectile* projectile in pList)
	{
		// update projectile
		projectile->Update(dt);
		
		// testing collision
		for each(baseEnemyShip* enemy in pEnemies)
		{
			if(projectile->GetMeshBox().Intersects(enemy->GetMeshBox()))
			{
				projectile->Destroy();
				enemy->calculateDamage(player.GetAttackPower());
				if(enemy->getHealth() <= 0)
				{
					enemy->destroyShip();
					enemiesSpawned --;
					player.IncrScore(enemy->getShipScoreWorth());

					PSys->play = true;
					D3DXMATRIX worldMat;
					D3DXMatrixTranslation(&worldMat, 
						enemy->getPosition().x, enemy->getPosition().y, enemy->getPosition().z);
					PSys->SetWorldMat(worldMat);
				}
			}
		}
	}

	// check for projectiles that need to be removed from the list
	// and remove them
	for(list<Projectile*>::const_iterator i = pList.begin(), end = pList.end(); i != end;)
	{	
		if((*i)->CheckObject())
		{
			delete (*i);
			i = pList.erase(i);
		}
		else
			i++;		
	}
	for(list<baseEnemyShip*>::const_iterator i = pEnemies.begin(), end = pEnemies.end(); i != end;)
	{
		if((*i)->CheckObject())
		{
			delete(*i);
			i = pEnemies.erase(i);
		}
		else
			i++;
	}

	//end the game if the player is out of health and lives
	if(player.GetCurrHlth() <= 0 && player.GetNumLives() <= 0)
	{
		//update global score
		Initializer::GetInstance()->setfinalscore(player.GetScore());
		gameState = GameOver;
	}

	//add win case for player here

	
	
}

void GameScreen::Render(void)
{
	// Set Z render state to false to ignore Z-coordinates when
	// rendering the sky box
	Initializer::GetInstance()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);

	// Render the sky box
	skybox.Render();

	// Set Z render state to true when finishing rendering 
	// the sky box
	Initializer::GetInstance()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);

	// Render player ship
	player.Render(shader);

	// Render particle system
	if(PSys->play)
		PSys->Render();

	for each(Projectile* projectile in pList)
	{

			projectile->Render(shader);
		
	}

	// render enemy
	for each(baseEnemyShip* enemy in pEnemies)
	{
		if(!enemy->GetIsHidden()/* && enemy->hasSpawned*/)
		{
			enemy->Render(shader);
			enemy->renderBullet(shader);
	
		}
	}

}

void GameScreen::Shutdown(void)
{
	// Delete Test variables
	delete PSys;
	PSys = NULL;
	projSFX->release();
	SAFE_RELEASE(shader);
	SAFE_RELEASE(errorCheck);

	if(pList.size() > 0)
	{
		for(list<Projectile*>::const_iterator i = pList.begin(), 
			end = pList.end(); i != end; )
		{
			delete (*i);
			i = pList.erase(i);
		}
	}

	if(pEnemies.size() > 0)
	{
		for(list<baseEnemyShip*>::const_iterator i = pEnemies.begin(), 
			end = pEnemies.end(); i != end; )
		{
			delete (*i);
			i = pEnemies.erase(i);
		}
	}
}

