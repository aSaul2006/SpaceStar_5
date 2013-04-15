#include "GameScreen.h"


GameScreen::GameScreen(void)
{
	type = GameType;
	Initialize();
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

	// initialize enemy
	enemy.initializeEnemyShip(L"viperShip.x");
	enemy.setSpeed(5.0);
	enemy.setPosition(D3DXVECTOR3(8.0f,0.0f, 0.0f));
	enemy.setAttackType(ATTACK1);
}

void GameScreen::Update(GameState& gameState, float dt)
{
	Camera::GetInstance()->Update(dt);
	player.Update(dt);
	enemy.update(dt,&player);

	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_SPACE))
	{
		pList.push_front(new Projectile(player.GetPosition(), D3DXVECTOR3(10.0f, 0, 0)));
	}

	// update the projectiles in pList
	for each(Projectile* projectile in pList)
	{
		// update projectile
		projectile->Update(dt);

		// testing collision
		if(projectile->GetMeshBox().Intersects(enemy.GetMeshBox()))
		{
			projectile->Destroy();
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

	for each(Projectile* projectile in pList)
	{
		projectile->Render(shader);
	}

	// render enemy
	enemy.Render(shader);

	enemy.renderBullet(shader);
}

void GameScreen::Shutdown(void)
{
	// Delete Test variables
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
}