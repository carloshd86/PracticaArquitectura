#ifndef __GAME_H__
#define __GAME_H__

#include "core.h"
#include "graphicsengine.h"
#include <vector>


class Entity;
class GameInputManager;
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
	GameLevel                                             m_gameLevel;
};

#endif
