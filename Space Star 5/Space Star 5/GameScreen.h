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
#include "AttackWaves.h"
#include "PSystem.h"
#include "ItemActor.h"
#include "ItemTypes.h"

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
	list<baseEnemyShip*> pEnemies;
	list<AttackWaves*> pWave;
	list<ItemActor*> pItemsDropped;
	list<PSystem*> pPSys;
	Viper viper;

	double spawnTime;
	int enemiesSpawned;

	FMOD::Sound* projSFX;	// SFX for the player's projectile

	int enemyDestroyedCount;
	AABB psysBox;

	//drop rate....does this even make sense lol
	int track;

public:
	GameScreen(void);
	~GameScreen(void);

	void Initialize(void);
	void Update(GameState& gameState, float dt);
	void Render(void);
	void Shutdown(void);

	int GetFinalScore() {return player.GetScore();}

	int GetRandDropRate(){return ((rand() % 80 + 40) - (rand() % 15 + 5));}
};

