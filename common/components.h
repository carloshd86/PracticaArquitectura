#ifndef __COMPONENTS_H_
#define __COMPONENTS_H_


#include "core.h"
#include "entity.h"
#include "sys.h"
#include "sprite.h"
#include "messages.h"
#include "game.h"
#include "graphicsengine.h"
#include <assert.h>


class Component 
{
public:

	Component(Entity * owner) :
		m_owner(owner) {}
	~Component() {}

	Entity * m_owner;

	virtual void Run(float deltaTime) = 0;
};

/***************************/

class C_Renderable : public Component, public IMessageReceiver
{
public:

	C_Renderable(Entity * owner, vec2 pos, vec2 size, const char * image) :
		Component     (owner),
		m_pos         (pos),
		m_size        (size),
		m_image       (image),
	    m_initialized (false) {}

	Sprite * GetSprite () const { return m_pSprite; }
	virtual void Run   (float deltaTime) {}
	
	void Init()
	{
		assert(g_pGame);
		if (!m_initialized)
		{
			m_pSprite = g_pGame->GetGraphicsEngine()->RequireSprite(m_pos, m_size, m_image);
		}
	}


	vec2 GetPos() const
	{
		return m_pos;
	}

	void SetPos(float x, float y)
	{
		m_pos.x = x;
		m_pos.y = y;
		m_pSprite->SetPos(x, y);
	}

	vec2 GetSize() const
	{
		return m_size;
	}

	void SetSize(float x, float y)
	{
		m_size.x = x;
		m_size.y = y;
		m_pSprite->SetSize(x, y);
	}

	void ReceiveMessage(GameMessage &message)
	{
		SetPositionMessage * spm = dynamic_cast<SetPositionMessage *>(&message);
		if (spm)
		{
			SetPos(spm->GetX(), spm->GetY());
		}
		else
		{
			AddPositionMessage * apm = dynamic_cast<AddPositionMessage *>(&message);
			if (apm)
			{
				SetPos(m_pos.x + apm->GetX(), m_pos.y + apm->GetY());
			}
			else
			{
				SetSizeMessage * ssm = dynamic_cast<SetSizeMessage *>(&message);
				if (ssm)
				{
					SetSize(ssm->GetX(), ssm->GetY());
				}
				else
				{
					RequirePositionMessage * rpm = dynamic_cast<RequirePositionMessage *>(&message);
					if (rpm)
					{
						rpm->SetX(m_pos.x);
						rpm->SetY(m_pos.y);
						rpm->SetProcessed(true);
					}
					else
					{
						RequireSizeMessage * rsm = dynamic_cast<RequireSizeMessage *>(&message);
						if (rsm)
						{
							rsm->SetX(m_size.x);
							rsm->SetY(m_size.y);
							rsm->SetProcessed(true);
						}
						else
						{
							ChangeSpriteMessage * csm = dynamic_cast<ChangeSpriteMessage *>(&message);
							if (csm)
							{
								const char * requiredTexture = csm->GetImage();
								if (strcmp(requiredTexture, m_image)) {
									// TODO cuidado con esto
									m_image = requiredTexture;
									g_pGame->GetGraphicsEngine()->ReleaseSprite(m_pSprite);
									m_pSprite = g_pGame->GetGraphicsEngine()->RequireSprite(m_pos, m_size, m_image);
								}
							}
						}
					}
				}
			}
		}
	}

private:
	
	vec2        m_pos;
	vec2        m_size;
	const char *m_image;
	Sprite     *m_pSprite;
	bool        m_initialized;
};

/***************************/

class C_Movable : public Component, public IMessageReceiver
{
public:

	C_Movable(Entity * owner, float speed) : 
		Component  (owner),
		m_movement { 0, 0 },
		m_speed    (speed) {}

	virtual void Run(float deltaTime)
	{
		if (m_movement.x || m_movement.y)
		{
			m_movement.x *= deltaTime;
			m_movement.y *= deltaTime;

			AddPositionMessage apm(m_movement.x, m_movement.y);
			m_owner->ReceiveMessage(apm);
		}
	}

	vec2 GetMovement() const
	{
		return m_movement;
	}

	void SetMovement(float x, float y)
	{
		m_movement.x = x;
		m_movement.y = y;
	}

	vec2 GetSpeed() const
	{
		return m_movement;
	}

	void SetSpeed(float speed)
	{
		m_speed = speed;
	}

	void MoveUp()
	{
		m_movement.y = m_speed;
	}

	void MoveDown()
	{
		m_movement.y = -m_speed;
	}

	void MoveLeft()
	{
		m_movement.x = -m_speed;
	}

	void MoveRight()
	{
		m_movement.x = m_speed;
	}

	void ReceiveMessage(GameMessage &message)
	{
		RigidBodyCollisionMessage * wcm = dynamic_cast<RigidBodyCollisionMessage *>(&message);
		if (wcm)
		{
			if      (RigidBodyCollisionMessage::Type::CollisionX == wcm->GetType()) m_owner->ReceiveMessage(AddPositionMessage(-m_movement.x, 0.f));
			else if (RigidBodyCollisionMessage::Type::CollisionY == wcm->GetType()) m_owner->ReceiveMessage(AddPositionMessage(0.f, -m_movement.y));;
		}
		else
		{
			RequireMovementMessage * rmm = dynamic_cast<RequireMovementMessage *>(&message);
			if (rmm)
			{
				rmm->SetX(m_movement.x);
				rmm->SetY(m_movement.y);
				rmm->SetSpeed(m_speed);
				rmm->SetProcessed(true);
			}
			else
			{
				SetMovementMessage * smm = dynamic_cast<SetMovementMessage *>(&message);
				if (smm)
				{
					m_movement.x = smm->GetX();
					m_movement.y = smm->GetY();
				}
				else
				{
					MoveUpMessage * mum = dynamic_cast<MoveUpMessage *>(&message);
					if (mum)
					{
						m_movement.y = m_speed;
					}
					else
					{
						MoveDownMessage  * mdm = dynamic_cast<MoveDownMessage *>(&message);
						if (mdm)
						{
							m_movement.y = -m_speed;
						}
						else
						{
							MoveLeftMessage  * mlm = dynamic_cast<MoveLeftMessage *>(&message);
							if (mlm)
							{
								m_movement.x = -m_speed;
							}
							else
							{
								MoveRightMessage * mrm = dynamic_cast<MoveRightMessage *>(&message);
								if (mrm)
								{
									m_movement.x = m_speed;
								}
							}
						}
					}	
				}
			}
		}
	}

private:

	vec2 m_movement;
	float m_speed;
};

/***************************/

class C_Player : public Component
{
public:

	C_Player(Entity * owner) :
		Component(owner) {}

	virtual void Run(float deltaTime)
	{
		//if (m_pos.x < 0)          m_pos.x = 0;
		//if (m_pos.y < 0)          m_pos.y = 0;
		//if (m_pos.x > SCR_WIDTH)  m_pos.x = SCR_WIDTH;
		//if (m_pos.y > SCR_HEIGHT) m_pos.y = SCR_HEIGHT;
	}
};

/***************************/

class C_Enemy : public Component, public IMessageReceiver
{

public:

	enum State
	{
		CES_PATROLLING,
		CES_PURSUING
	};

	C_Enemy(Entity * owner, float pursuingSpeed) :
		Component       (owner),
		m_pursuingSpeed (pursuingSpeed),
		m_state         (CES_PATROLLING) {}

	virtual void Run(float deltaTime)
	{
		assert(g_pGame);		
		Entity * player = g_pGame->GetPlayer();

		RequirePositionMessage       positionMessage;
		RequireSizeMessage           sizeMessage;
		RequireMovementMessage       movementMessage;
		RequirePositionMessage       playerPositionMessage;
		RequireSizeMessage           playerSizeMessage;

		m_owner->ReceiveMessage(positionMessage);
		m_owner->ReceiveMessage(sizeMessage);
		m_owner->ReceiveMessage(movementMessage);
		player->ReceiveMessage(playerPositionMessage);
		player->ReceiveMessage(playerSizeMessage);

		assert(positionMessage.GetProcessed());
		assert(sizeMessage.GetProcessed());
		assert(movementMessage.GetProcessed());
		assert(playerPositionMessage.GetProcessed());
		assert(playerSizeMessage.GetProcessed());

		RequireRouteMessage routeMessage;
		m_owner->ReceiveMessage(routeMessage);

		vec2    pos                              = vmake(positionMessage.GetX()       , positionMessage.GetY());
		vec2    size                             = vmake(sizeMessage.GetX()           , sizeMessage.GetY());
		vec2    movement                         = vmake(movementMessage.GetX()       , movementMessage.GetY());
		float   speed                            = movementMessage.GetSpeed();
		vec2    playerPos                        = vmake(playerPositionMessage.GetX() , playerPositionMessage.GetY());
		vec2    playerSize                       = vmake(playerSizeMessage.GetX()     , playerSizeMessage.GetY());
		const   std::vector<vec2> * routePoints  = routeMessage.GetRoutePoints();
		uint8_t currentRoutePoint                = routeMessage.GetCurrentRoutePoint();

		if (CES_PATROLLING == m_state && routePoints && !routePoints->empty())
		{
			vec2 nextRoutePoint = (*routePoints)[currentRoutePoint];

			vec2 direction;

			float angle = atan2(nextRoutePoint.y - pos.y, nextRoutePoint.x - pos.x);
			direction = vunit(angle);
			
			/*SetMovementMessage smm(speed * deltaTime * direction.x, speed * deltaTime * direction.y);*/
			SetMovementMessage smm(speed * direction.x, speed * direction.y);
			m_owner->ReceiveMessage(smm);
			//SetPos(m_pos.x + m_movement.x, m_pos.y + m_movement.y);


			m_owner->ReceiveMessage(IncreaseCurrentRoutePointMessage(pos.x, pos.y));

			//if (m_pos.x < 0)             { SetPos(0.f, m_pos.y); }
			//if (m_pos.x > SCR_WIDTH)     { SetPos(SCR_WIDTH, m_pos.y); }


			bool enemy_sights = false;
			if (movement.x < 0 && playerPos.x < pos.x)
			{
				if ((playerPos.y <= (pos.y + size.y) && playerPos.y >= pos.y)
					|| ((playerPos.y + playerSize.y) <= (pos.y + size.y) && (playerPos.y + playerSize.y) >= pos.y))
				{
					enemy_sights = true;
					OutputDebugString("Enemy sights player\n");
					m_state = CES_PURSUING;
				}
			}
			else if (movement.x > 0 && playerPos.x > pos.x)
			{
				if ((playerPos.y <= (pos.y + size.y) && playerPos.y >= pos.y)
					|| ((playerPos.y + playerSize.y) <= (pos.y + size.y) && (playerPos.y + playerSize.y) >= pos.y))
				{
					enemy_sights = true;
					OutputDebugString("Enemy sights player\n");
					m_state = CES_PURSUING;
				}
			}
		} 
		else
		{
			OutputDebugString("Enemy pursuing player\n");

			float angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
			vec2 direction = vunit(angle);
			/*movement.x = PLAYER_SPEED * deltaTime * direction.x;
			movement.y = PLAYER_SPEED * deltaTime * direction.y;*/
			//SetPos(pos.x + movement.x, pos.y + movement.y);
			/*SetMovementMessage smm(m_pursuingSpeed * deltaTime * direction.x, m_pursuingSpeed * deltaTime * direction.y);*/
			SetMovementMessage smm(m_pursuingSpeed * direction.x, m_pursuingSpeed * direction.y);
			m_owner->ReceiveMessage(smm);
		}

		const char * required_texture = ((movement.x < 0) ? "../data/soldier_left.png" : "../data/soldier_right.png");
		ChangeSpriteMessage csm(required_texture);
		m_owner->ReceiveMessage(csm);
	}

	void ReceiveMessage(GameMessage &message)
	{
		RequirePursuingSpeedMessage * rpsm = dynamic_cast<RequirePursuingSpeedMessage *>(&message);
		if (rpsm)
		{
			rpsm->SetPursuingSpeed(m_pursuingSpeed);
			rpsm->SetProcessed(true);
		}
	}

private:

	State m_state;
	float m_pursuingSpeed;
};

/***************************/

class C_RoutePath : public Component, public IMessageReceiver
{
public:
	
	C_RoutePath(Entity * owner) :
		Component           (owner),
		m_currentRoutePoint (0) {}

	virtual void Run(float deltaTime)
	{
		
	}

	void AddRoutePoint(float x, float y)
	{
		m_routePoints.push_back(vmake(x, y));
	}

	void RemoveRoutePoint(float x, float y)
	{
		for (auto it = m_routePoints.begin(); it != m_routePoints.end(); ) 
		{
			if ((*it).x == x && (*it).y == y) 
			{
				it = m_routePoints.erase(it);
				break;
			} 
			else 
			{
				++it;
			}
		}
	}

	void ClearRoutePoints()
	{
		m_routePoints.clear();
	}

	void ReceiveMessage(GameMessage &message)
	{
		RequireRouteMessage * rrm = dynamic_cast<RequireRouteMessage *>(&message);
		if (rrm)
		{
			rrm->SetCurrentRoutePoint(m_currentRoutePoint);
			rrm->SetRoutePoints(&m_routePoints);
		}
		else
		{
			IncreaseCurrentRoutePointMessage * icrpm = dynamic_cast<IncreaseCurrentRoutePointMessage *>(&message);
			if (icrpm)
			{ 
				if (abs(m_routePoints[m_currentRoutePoint].x - icrpm->GetCurrentX()) < 2 && abs(m_routePoints[m_currentRoutePoint].y - icrpm->GetCurrentY()) < 2)
					++m_currentRoutePoint;
				if(m_currentRoutePoint >= m_routePoints.size())
					m_currentRoutePoint = 0;
			}
		}
	}

private:

	std::vector<vec2> m_routePoints;
	uint8_t           m_currentRoutePoint;
};

/***************************/

class C_RigidBody : public Component
{
public:

	C_RigidBody(Entity * owner) :
		Component(owner) {}

	virtual void Run(float deltaTime)
	{
	}
};

/***************************/

class C_Goal : public Component
{
public:

	C_Goal(Entity * owner) :
		Component(owner) {}

	virtual void Run(float deltaTime)
	{
	}
};

#endif

