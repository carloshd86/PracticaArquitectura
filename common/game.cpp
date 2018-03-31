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
	m_initialized     (false), 
	m_ended           (false), 
	m_pPlayer         (nullptr), 
	m_pGoal           (nullptr),
	m_pInputManager   (nullptr) {}

Game::~Game()
{
	if (m_initialized && !m_ended) End();
}

void Game::Init()
{
	if (!m_initialized)
	{
		// Init game state
		char * jsonSrc = nullptr;
		switch (m_gameLevel)
		{
			case LEVEL_1: jsonSrc = "../data/level1.json"; break;
			case LEVEL_2: jsonSrc = "../data/level2.json"; break;
			case LEVEL_3: jsonSrc = "../data/level3.json"; break;
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
		playerImgSrc += playerImg;

		m_pPlayer = new Entity();
		Component * cPlayer              = new C_Player(m_pPlayer);
		C_Renderable * cPlayerRenderable = new C_Renderable(m_pPlayer, vmake(playerPosX, playerPosY), vmake(playerWidth, playerHeight), playerImgSrc.c_str());
		cPlayerRenderable->Init();
		C_Movable * cPlayerMovable       = new C_Movable(m_pPlayer, playerSpeed);
		g_pEventManager->Register(cPlayerMovable, IEventManager::EM_Event::MoveUp    , 0);
		g_pEventManager->Register(cPlayerMovable, IEventManager::EM_Event::MoveDown  , 0);
		g_pEventManager->Register(cPlayerMovable, IEventManager::EM_Event::MoveLeft  , 0);
		g_pEventManager->Register(cPlayerMovable, IEventManager::EM_Event::MoveRight , 0);
		m_pPlayer->AddComponent(cPlayer);
		m_pPlayer->AddComponent(cPlayerRenderable);
		m_pPlayer->AddComponent(cPlayerMovable);
		m_entities.push_back(m_pPlayer);


		m_pInputManager = new GameInputManager();
	
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
		goalImgSrc += goalImg;

		m_pGoal = new Entity();
		Component * cGoal              = new C_Goal(m_pGoal);
		C_Renderable * cGoalRenderable = new C_Renderable(m_pGoal, vmake(goalPosX, goalPosY), vmake(goalWidth, goalHeight), goalImgSrc.c_str());
		cGoalRenderable->Init();
		m_pGoal->AddComponent(cGoal);
		m_pGoal->AddComponent(cGoalRenderable);
		m_entities.push_back(m_pGoal);

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
				enemyImgSrc += enemyImg;

				Entity * enemy = new Entity();
				Component * cEnemy              = new C_Enemy(enemy, enemyPursuingSpeed);
				C_Renderable * cEnemyRenderable = new C_Renderable(enemy, vmake(enemyPosX, enemyPosY), vmake(enemyWidth, enemyHeight), enemyImgSrc.c_str());
				cEnemyRenderable->Init();
				Component * cEnemyMovable       = new C_Movable(enemy, enemySpeed);
				enemy->AddComponent(cEnemy);
				enemy->AddComponent(cEnemyRenderable);
				enemy->AddComponent(cEnemyMovable);
	
				if (jsonEnemy.HasMember(ROUTE_POINTS_JSON_KEY))
				{
					Component * cRoutePath = new C_RoutePath(enemy);
					for (auto& point : jsonEnemy[ROUTE_POINTS_JSON_KEY].GetArray())
					{
						auto& routePoint = point.GetObjectA();

						assert(routePoint.HasMember(POS_X_JSON_KEY));
						assert(routePoint.HasMember(POS_Y_JSON_KEY));

						float routePointX = routePoint[POS_X_JSON_KEY].GetFloat();
						float routePointY = routePoint[POS_Y_JSON_KEY].GetFloat();

						reinterpret_cast<C_RoutePath *>(cRoutePath)->AddRoutePoint(routePointX, routePointY);
					}
					enemy->AddComponent(cRoutePath);
				}

				m_entities.push_back(enemy);
				m_enemies.push_back(enemy);
			}

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
				wallImgSrc += wallImg;

				Entity * wall = new Entity();
				Component * cWall              = new C_RigidBody(wall);
				C_Renderable * cWallRenderable = new C_Renderable(wall, vmake(wallPosX, wallPosY), vmake(wallWidth, wallHeight), wallImgSrc.c_str());
				cWallRenderable->Init();
				wall->AddComponent(cWall);
				wall->AddComponent(cWallRenderable);

				m_entities.push_back(wall);
				m_walls.push_back(wall);
			}

		m_initialized = true;
	}
}

void Game::End()
{
	if (m_initialized)
	{
		for(auto entity : m_entities) { delete entity; }
		m_entities.clear();
		
		m_pPlayer = nullptr;
		m_pGoal   = nullptr;

		m_walls.clear();
		m_enemies.clear();

		delete m_pInputManager;
		m_pInputManager = nullptr;

		m_initialized = false;
	}
	m_ended = true;
}


void Game::Run(float deltaTime)
{
	for (auto entity : m_entities) entity->Run(deltaTime);
	MoveEntities();
	CheckCollisions();
}

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

	for (auto wall : m_walls)
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
				m_pPlayer->ReceiveMessage(RigidBodyCollisionMessage(RigidBodyCollisionMessage::Type::CollisionX));
			}

			playerPosCheck.x = playerPos.x - playerMovement.x;
			playerPosCheck.y = playerPos.y;
			if (CheckRectCollision(playerPosCheck, playerSize, wallPos, wallSize))
			{
				m_pPlayer->ReceiveMessage(RigidBodyCollisionMessage(RigidBodyCollisionMessage::Type::CollisionY));
			}
		}

		// Enemy with wall
		for (auto enemy : m_enemies)
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
					enemy->ReceiveMessage(RigidBodyCollisionMessage(RigidBodyCollisionMessage::Type::CollisionX));
				}

				if (CheckRectCollision(enemyPos, enemySize, wallPos, wallSize))
				{
					enemy->ReceiveMessage(RigidBodyCollisionMessage(RigidBodyCollisionMessage::Type::CollisionY));
				}
			}
		}

	}
}

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

		g_pApplicationManager->SwitchMode(AM_MENU);
	}

	for (auto enemy : m_enemies)
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

			g_pApplicationManager->SwitchMode(AM_MENU);
			break;
		}
	}
}

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

GameInputManager * Game::GetInputManager() const
{
	return m_pInputManager;
}

Entity * Game::GetPlayer() const
{
	return m_pPlayer;
}

Game::GameLevel Game::GetGameLevel() const 
{
	return m_gameLevel;
}

void Game::SetGameLevel(GameLevel level)
{
	m_gameLevel = level;
}