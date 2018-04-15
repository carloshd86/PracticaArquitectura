#ifndef __GAME_H__
#define __GAME_H__

#include "core.h"
#include "graphicsengine.h"
#include <vector>


class Entity;
class GameInputManager;

class Game
{
public:

	enum GameLevel
	{
		Level1,
		Level2,
		Level3
	};

	Game();
	~Game();

	void Init ();
	void End  ();
	void Run  (float deltaTime);

	Entity           * GetPlayer       () const;
	GameLevel          GetGameLevel    () const;
	void               SetGameLevel    (GameLevel level);

private:

	void MoveEntities       ();
	void CheckCollisions    ();
	bool CheckRectCollision (vec2 pos1, vec2 size1, vec2 pos2, vec2 size2);
	
	bool                   mInitialized;
	bool                   mEnded;
	std::vector<Entity *>  mEntities;
	Entity                *m_pPlayer;
	std::vector<Entity *>  mWalls;
	std::vector<Entity *>  mEnemies;
	Entity                *m_pGoal;
	GameLevel              mGameLevel;
};

#endif
