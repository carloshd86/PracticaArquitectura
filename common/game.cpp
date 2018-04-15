#include "stdafx.h"
#include "globals.h"
#include "game.h"
#include "applicationmanager.h"
#include "entity.h"
#include "gameinputmanager.h"
#include "messagereceiver.h"
#include "messages.h"
#include "rapidjson\document.h"
#include "rapidjson/error/en.h"
#include <assert.h>
#include <fstream>
#include "components.h"


using namespace rapidjson;


Game::Game() : 
	mInitialized     (false), 
	mEnded           (false), 
	m_pPlayer         (nullptr), 
	m_pGoal           (nullptr) {}

// *************************************************
//
// *************************************************

Game::~Game()
{
	if (mInitialized && !mEnded) End();
}

// *************************************************
//
// *************************************************

void Game::Init()
{
	if (!mInitialized)
	{
		// Init game state
		char * jsonSrc = nullptr;
		switch (mGameLevel)
		{
			case Level1: jsonSrc = "../data/level1.json"; break;
			case Level2: jsonSrc = "../data/level2.json"; break;
			case Level3: jsonSrc = "../data/level3.json"; break;
		}

		assert(jsonSrc);

		std::ifstream file(jsonSrc, std::ios::binary | std::ios::ate);
		assert(file.is_open());

		std::vector<char> json(static_cast<uint16_t>(file.tellg()));
		file.seekg(0);
		file.read(json.data(), json.size());
		json.push_back('\0');

		Document document;
		ParseResult ok = document.Parse(json.data());
		if (!ok) {
			OutputDebugStringA(GetParseError_En(ok.Code()));
			OutputDebugStringA(std::to_string(ok.Offset()).c_str());
			exit(EXIT_FAILURE);
		}
		assert(document.IsObject());
	
		// Level
		assert(document.HasMember(LEVEL_JSON_KEY));
		assert(document[LEVEL_JSON_KEY].IsObject());
		assert(document[LEVEL_JSON_KEY].HasMember(BACKGROUND_IMG_JSON_KEY));

		const char * backgroundImg    = document[LEVEL_JSON_KEY][BACKGROUND_IMG_JSON_KEY].GetString();
		std::string  backgroundImgSrc = "../data/";
		backgroundImgSrc.append(backgroundImg);
		float backgroundR = 1.f;
		float backgroundG = 1.f;
		float backgroundB = 1.f;
		if (document[LEVEL_JSON_KEY].HasMember(BACKGROUND_R_JSON_KEY))
			backgroundR = document[LEVEL_JSON_KEY][BACKGROUND_R_JSON_KEY].GetFloat();
		if (document[LEVEL_JSON_KEY].HasMember(BACKGROUND_G_JSON_KEY))
			backgroundG = document[LEVEL_JSON_KEY][BACKGROUND_G_JSON_KEY].GetFloat();
		if (document[LEVEL_JSON_KEY].HasMember(BACKGROUND_B_JSON_KEY))
			backgroundB = document[LEVEL_JSON_KEY][BACKGROUND_B_JSON_KEY].GetFloat();
		g_pGraphicsEngine->SetBackgroundImage(backgroundImgSrc.c_str());
		g_pGraphicsEngine->SetBackgroundColor(backgroundR, backgroundG, backgroundB);

		// Player
		assert(document.HasMember(PLAYER_JSON_KEY));
		assert(document[PLAYER_JSON_KEY].IsObject());
		assert(document[PLAYER_JSON_KEY].HasMember(POS_X_JSON_KEY));
		assert(document[PLAYER_JSON_KEY].HasMember(POS_Y_JSON_KEY));
		assert(document[PLAYER_JSON_KEY].HasMember(WIDTH_JSON_KEY));
		assert(document[PLAYER_JSON_KEY].HasMember(HEIGHT_JSON_KEY));
		assert(document[PLAYER_JSON_KEY].HasMember(SPEED_JSON_KEY));
		assert(document[PLAYER_JSON_KEY].HasMember(IMG_JSON_KEY));

		float        playerPosX   = document[PLAYER_JSON_KEY][POS_X_JSON_KEY].GetFloat();
		float        playerPosY   = document[PLAYER_JSON_KEY][POS_Y_JSON_KEY].GetFloat();
		float        playerWidth  = document[PLAYER_JSON_KEY][WIDTH_JSON_KEY].GetFloat();
		float        playerHeight = document[PLAYER_JSON_KEY][HEIGHT_JSON_KEY].GetFloat();
		float        playerSpeed  = document[PLAYER_JSON_KEY][SPEED_JSON_KEY].GetFloat();
		const char * playerImg    = document[PLAYER_JSON_KEY][IMG_JSON_KEY].GetString();
		std::string  playerImgSrc = "../data/";
		playerImgSrc.append(playerImg);

		m_pPlayer = new Entity();
		C_Player * cPlayer                    = new C_Player(m_pPlayer);
		cPlayer->Init();
		C_Renderable * cPlayerRenderable      = new C_Renderable(m_pPlayer, vmake(playerPosX, playerPosY), vmake(playerWidth, playerHeight), playerImgSrc.c_str());
		cPlayerRenderable->Init();
		C_Movable      * cPlayerMovable       = new C_Movable(m_pPlayer, playerSpeed);
		cPlayerMovable->Init();
		C_Controllable * cPlayerControllable  = new C_Controllable(m_pPlayer);
		cPlayerControllable->Init();
		m_pPlayer->AddComponent(cPlayer);
		m_pPlayer->AddComponent(cPlayerRenderable);
		m_pPlayer->AddComponent(cPlayerMovable);
		m_pPlayer->AddComponent(cPlayerControllable);
		mEntities.push_back(m_pPlayer);

		// Goal
		assert(document.HasMember(GOAL_JSON_KEY));
		assert(document[GOAL_JSON_KEY].IsObject());
		assert(document[GOAL_JSON_KEY].HasMember(POS_X_JSON_KEY));
		assert(document[GOAL_JSON_KEY].HasMember(POS_Y_JSON_KEY));
		assert(document[GOAL_JSON_KEY].HasMember(WIDTH_JSON_KEY));
		assert(document[GOAL_JSON_KEY].HasMember(HEIGHT_JSON_KEY));
		assert(document[GOAL_JSON_KEY].HasMember(IMG_JSON_KEY));

		float        goalPosX   = document[GOAL_JSON_KEY][POS_X_JSON_KEY].GetFloat();
		float        goalPosY   = document[GOAL_JSON_KEY][POS_Y_JSON_KEY].GetFloat();
		float        goalWidth  = document[GOAL_JSON_KEY][WIDTH_JSON_KEY].GetFloat();
		float        goalHeight = document[GOAL_JSON_KEY][HEIGHT_JSON_KEY].GetFloat();
		const char * goalImg    = document[GOAL_JSON_KEY][IMG_JSON_KEY].GetString();
		std::string  goalImgSrc = "../data/";
		goalImgSrc.append(goalImg);

		m_pGoal = new Entity();
		C_Goal * cGoal                 = new C_Goal(m_pGoal);
		cGoal->Init();
		C_Renderable * cGoalRenderable = new C_Renderable(m_pGoal, vmake(goalPosX, goalPosY), vmake(goalWidth, goalHeight), goalImgSrc.c_str());
		cGoalRenderable->Init();
		m_pGoal->AddComponent(cGoal);
		m_pGoal->AddComponent(cGoalRenderable);
		mEntities.push_back(m_pGoal);

		// Enemies
		if (document.HasMember(ENEMIES_JSON_KEY))
			for (auto& item : document[ENEMIES_JSON_KEY].GetArray())
			{
				auto& jsonEnemy = item.GetObjectA();

				assert(jsonEnemy.HasMember(POS_X_JSON_KEY));
				assert(jsonEnemy.HasMember(POS_Y_JSON_KEY));
				assert(jsonEnemy.HasMember(WIDTH_JSON_KEY));
				assert(jsonEnemy.HasMember(HEIGHT_JSON_KEY));
				assert(jsonEnemy.HasMember(SPEED_JSON_KEY));
				assert(jsonEnemy.HasMember(PURSUING_SPEED_JSON_KEY));
				assert(jsonEnemy.HasMember(IMG_JSON_KEY));

				float        enemyPosX           = jsonEnemy[POS_X_JSON_KEY].GetFloat();
				float        enemyPosY           = jsonEnemy[POS_Y_JSON_KEY].GetFloat();
				float        enemyWidth          = jsonEnemy[WIDTH_JSON_KEY].GetFloat();
				float        enemyHeight         = jsonEnemy[HEIGHT_JSON_KEY].GetFloat();
				float        enemySpeed          = jsonEnemy[SPEED_JSON_KEY].GetFloat();
				float        enemyPursuingSpeed  = jsonEnemy[PURSUING_SPEED_JSON_KEY].GetFloat();
				const char * enemyImg            = jsonEnemy[IMG_JSON_KEY].GetString();
				std::string  enemyImgSrc         = "../data/";
				enemyImgSrc.append(enemyImg);

				Entity * enemy = new Entity();
				C_Enemy * cEnemy                = new C_Enemy(enemy, enemyPursuingSpeed);
				cEnemy->Init();
				C_Renderable * cEnemyRenderable = new C_Renderable(enemy, vmake(enemyPosX, enemyPosY), vmake(enemyWidth, enemyHeight), enemyImgSrc.c_str());
				cEnemyRenderable->Init();
				C_Movable * cEnemyMovable       = new C_Movable(enemy, enemySpeed);
				cEnemyMovable->Init();
				enemy->AddComponent(cEnemy);
				enemy->AddComponent(cEnemyRenderable);
				enemy->AddComponent(cEnemyMovable);
	
				if (jsonEnemy.HasMember(ROUTE_POINTS_JSON_KEY))
				{
					C_RoutePath * cRoutePath = new C_RoutePath(enemy);
					cRoutePath->Init();
					for (auto& point : jsonEnemy[ROUTE_POINTS_JSON_KEY].GetArray())
					{
						auto& routePoint = point.GetObjectA();

						assert(routePoint.HasMember(POS_X_JSON_KEY));
						assert(routePoint.HasMember(POS_Y_JSON_KEY));

						float routePointX = routePoint[POS_X_JSON_KEY].GetFloat();
						float routePointY = routePoint[POS_Y_JSON_KEY].GetFloat();

						cRoutePath->AddRoutePoint(routePointX, routePointY);
					}
					enemy->AddComponent(cRoutePath);
				}

				mEntities.push_back(enemy);
				mEnemies.push_back(enemy);
			}

		// Walls
		if (document.HasMember(WALLS_JSON_KEY))
			for (auto& item : document[WALLS_JSON_KEY].GetArray())
			{
				auto& jsonWall = item.GetObjectA();

				assert(jsonWall.HasMember(POS_X_JSON_KEY));
				assert(jsonWall.HasMember(POS_Y_JSON_KEY));
				assert(jsonWall.HasMember(WIDTH_JSON_KEY));
				assert(jsonWall.HasMember(HEIGHT_JSON_KEY));
				assert(jsonWall.HasMember(IMG_JSON_KEY));

				float        wallPosX   = jsonWall[POS_X_JSON_KEY].GetFloat();
				float        wallPosY   = jsonWall[POS_Y_JSON_KEY].GetFloat();
				float        wallWidth  = jsonWall[WIDTH_JSON_KEY].GetFloat();
				float        wallHeight = jsonWall[HEIGHT_JSON_KEY].GetFloat();
				const char * wallImg    = jsonWall[IMG_JSON_KEY].GetString();
				std::string  wallImgSrc = "../data/";
				wallImgSrc.append(wallImg);

				Entity * wall = new Entity();
				C_RigidBody * cWall            = new C_RigidBody(wall);
				cWall->Init();
				C_Renderable * cWallRenderable = new C_Renderable(wall, vmake(wallPosX, wallPosY), vmake(wallWidth, wallHeight), wallImgSrc.c_str());
				cWallRenderable->Init();
				wall->AddComponent(cWall);
				wall->AddComponent(cWallRenderable);

				mEntities.push_back(wall);
				mWalls.push_back(wall);
			}

		mInitialized = true;
	}
}

// *************************************************
//
// *************************************************

void Game::End()
{
	if (mInitialized)
	{
		for(auto entity : mEntities) { delete entity; }
		mEntities.clear();
		
		m_pPlayer = nullptr;
		m_pGoal   = nullptr;

		mWalls.clear();
		mEnemies.clear();

		mInitialized = false;
	}
	mEnded = true;
}

// *************************************************
//
// *************************************************

void Game::Run(float deltaTime)
{
	for (auto entity : mEntities) entity->Run(deltaTime);
	MoveEntities();
	CheckCollisions();
}

// *************************************************
//
// *************************************************

void Game::MoveEntities() 
{
	RequirePositionMessage positionMessage;
	m_pPlayer->ReceiveMessage(positionMessage);
	assert(positionMessage.GetProcessed());
	vec2 playerPos = vmake(positionMessage.GetX(), positionMessage.GetY());

	RequireSizeMessage sizeMessage;
	m_pPlayer->ReceiveMessage(sizeMessage);
	assert(sizeMessage.GetProcessed());
	vec2 playerSize = vmake(sizeMessage.GetX(), sizeMessage.GetY());

	RequireMovementMessage movementMessage;
	m_pPlayer->ReceiveMessage(movementMessage);
	assert(movementMessage.GetProcessed());
	vec2 playerMovement = vmake(movementMessage.GetX(), movementMessage.GetY());
	
	vec2 playerPosCheck;

	for (auto wall : mWalls)
	{
		positionMessage.SetProcessed(false);
		wall->ReceiveMessage(positionMessage);
		assert(positionMessage.GetProcessed());
		vec2 wallPos = vmake(positionMessage.GetX(), positionMessage.GetY());

		sizeMessage.SetProcessed(false);
		wall->ReceiveMessage(sizeMessage);
		assert(sizeMessage.GetProcessed());
		vec2 wallSize = vmake(sizeMessage.GetX(), sizeMessage.GetY());

		// Player with wall
		if (playerMovement.x || playerMovement.y)
		{
			playerPosCheck.x = playerPos.x;
			playerPosCheck.y = playerPos.y - playerMovement.y;
			if (CheckRectCollision(playerPosCheck, playerSize, wallPos, wallSize))
			{
				m_pPlayer->ReceiveMessage(RigidBodyCollisionMessage(RigidBodyCollisionMessage::RGBM_Type::CollisionX));
			}

			playerPosCheck.x = playerPos.x - playerMovement.x;
			playerPosCheck.y = playerPos.y;
			if (CheckRectCollision(playerPosCheck, playerSize, wallPos, wallSize))
			{
				m_pPlayer->ReceiveMessage(RigidBodyCollisionMessage(RigidBodyCollisionMessage::RGBM_Type::CollisionY));
			}
		}

		// Enemy with wall
		for (auto enemy : mEnemies)
		{
			movementMessage.SetProcessed(false);
			enemy->ReceiveMessage(movementMessage);
			assert(movementMessage.GetProcessed());
			vec2 enemyMovement = vmake(movementMessage.GetX(), movementMessage.GetY());
			if (enemyMovement.x || enemyMovement.y)
			{
				positionMessage.SetProcessed(false);
				enemy->ReceiveMessage(positionMessage);
				assert(positionMessage.GetProcessed());
				vec2 enemyPos = vmake(positionMessage.GetX(), positionMessage.GetY());

				sizeMessage.SetProcessed(false);
				enemy->ReceiveMessage(sizeMessage);
				assert(sizeMessage.GetProcessed());
				vec2 enemySize = vmake(sizeMessage.GetX(), sizeMessage.GetY());

				vec2 enemyPosCheck = vmake(enemyPos.x, enemyPos.y - enemyMovement.y);

				if (CheckRectCollision(enemyPosCheck, enemySize, wallPos, wallSize))
				{
					enemy->ReceiveMessage(RigidBodyCollisionMessage(RigidBodyCollisionMessage::RGBM_Type::CollisionX));
				}

				if (CheckRectCollision(enemyPos, enemySize, wallPos, wallSize))
				{
					enemy->ReceiveMessage(RigidBodyCollisionMessage(RigidBodyCollisionMessage::RGBM_Type::CollisionY));
				}
			}
		}

	}
}

// *************************************************
//
// *************************************************

void Game::CheckCollisions()
{
	RequirePositionMessage positionMessage;
	m_pPlayer->ReceiveMessage(positionMessage);
	assert(positionMessage.GetProcessed());
	vec2 playerPos = vmake(positionMessage.GetX(), positionMessage.GetY());

	RequireSizeMessage sizeMessage;
	m_pPlayer->ReceiveMessage(sizeMessage);
	assert(sizeMessage.GetProcessed());
	vec2 playerSize = vmake(sizeMessage.GetX(), sizeMessage.GetY());

	positionMessage.SetProcessed(false);
	m_pGoal->ReceiveMessage(positionMessage);
	assert(positionMessage.GetProcessed());
	vec2 goalPos = vmake(positionMessage.GetX(), positionMessage.GetY());

	sizeMessage.SetProcessed(false);
	m_pGoal->ReceiveMessage(sizeMessage);
	assert(sizeMessage.GetProcessed());
	vec2 goalSize = vmake(sizeMessage.GetX(), sizeMessage.GetY());

	if (CheckRectCollision(playerPos, playerSize, goalPos, goalSize))
	{
		OutputDebugString("Player reaches goal\n");
		m_pPlayer->ReceiveMessage(GoalReachedMessage());

		g_pApplicationManager->SwitchMode(AM_Menu);
	}

	for (auto enemy : mEnemies)
	{
		positionMessage.SetProcessed(false);
		enemy->ReceiveMessage(positionMessage);
		assert(positionMessage.GetProcessed());
		vec2 enemyPos = vmake(positionMessage.GetX(), positionMessage.GetY());

		sizeMessage.SetProcessed(false);
		enemy->ReceiveMessage(sizeMessage);
		assert(sizeMessage.GetProcessed());
		vec2 enemySize = vmake(sizeMessage.GetX(), sizeMessage.GetY());

		if (CheckRectCollision(playerPos, playerSize, enemyPos, enemySize))
		{
			OutputDebugString("Enemy hits player\n");
			m_pPlayer->ReceiveMessage(EnemyCollisionMessage());

			g_pApplicationManager->SwitchMode(AM_Menu);
			break;
		}
	}
}

// *************************************************
//
// *************************************************

bool Game::CheckRectCollision(vec2 pos1, vec2 size1, vec2 pos2, vec2 size2)
{
	bool collision = false;

	if (pos1.x <= (pos2.x + size2.x) && pos1.x >= pos2.x)
	{
		if      (pos1.y <= (pos2.y + size2.y) && pos1.y >= pos2.y)   collision = true;
		else if (pos2.y <= (pos1.y + size1.y) && pos2.y >= pos1.y)   collision = true;
	}
	else 
	{
		if (pos2.x <= (pos1.x + size1.x) && pos2.x >= pos1.x) 
		{
			if      (pos1.y <= (pos2.y + size2.y) && pos1.y >= pos2.y)   collision = true;
			else if (pos2.y <= (pos1.y + size1.y) && pos2.y >= pos1.y)   collision = true;
		}
	}

	return collision;
}

// *************************************************
//
// *************************************************

Entity * Game::GetPlayer() const
{
	return m_pPlayer;
}

// *************************************************
//
// *************************************************

Game::GameLevel Game::GetGameLevel() const 
{
	return mGameLevel;
}

// *************************************************
//
// *************************************************

void Game::SetGameLevel(GameLevel level)
{
	mGameLevel = level;
}