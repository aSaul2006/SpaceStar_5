#pragma once
#include "Screen.h"
#include "Camera.h"
#include "Player.h"
#include "EnemyShip.h"
#include "Skybox.h"
#include "Projectile.h"
#include "EnemyShip.h"
#include "AttackType.h"
#include "AudioManager.h"
#include "Misc\CrudeTimer.h"

class GameScreen : public CScreen
{
private:
	Player player;

	//////////////////////////////////////////////////////////////////////////////
	///// Test Variables - Used for testing
	//////////////////////////////////////////////////////////////////////////////
	ID3DXEffect* shader;
	ID3DXBuffer* errorCheck;
	int onScreenEnemies;
	Skybox skybox;

	list<Projectile*> pList;
	list<Enemy*> pEnemies;
	Enemy enemy;

	double spawnTime;
	int enemiesSpawned;

	FMOD::Sound* projSFX;	// SFX for the player's projectile
	
public:
	GameScreen(void);
	~GameScreen(void);

	void Initialize(void);
	void Update(GameState& gameState, float dt);
	void Render(void);
	void Shutdown(void);
};

