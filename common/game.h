#ifndef _GAME_H_
#define _GAME_H_

#include "core.h"
#include <vector>


class Entity;
class GameInputManager;
class GraphicsEngine;
class IMessageReceiver;
class GameMessage;

class Game
{
public:

	enum GameLevel
	{
		LEVEL_1,
		LEVEL_2,
		LEVEL_3
	};

	Game();
	~Game();

	void Init                                  ();
	void End                                   ();
	void Run                                   (float deltaTime);

	GameInputManager * GetInputManager   () const;
	GraphicsEngine   * GetGraphicsEngine () const;
	Entity           * GetPlayer         () const;
	GameLevel          GetGameLevel      () const;
	void               SetGameLevel      (GameLevel level);

private:

	void MoveEntities       ();
	void CheckCollisions    ();
	bool CheckRectCollision (vec2 pos1, vec2 size1, vec2 pos2, vec2 size2);
	
	bool                                                  m_initialized;
	bool                                                  m_ended;
	std::vector<Entity *>                                 m_entities;
	Entity                                               *m_pPlayer;
	std::vector<Entity *>                                 m_walls;
	std::vector<Entity *>                                 m_enemies;
	Entity                                               *m_pGoal;
	GameInputManager                                     *m_pInputManager;
	GraphicsEngine                                       *m_pGraphicsEngine;
	GameLevel                                             m_gameLevel;
};

#endif
