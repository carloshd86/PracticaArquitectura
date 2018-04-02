#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__


#include "entity.h"
#include "sprite.h"
#include "messages.h"
#include "game.h"
#include "graphicsengine.h"
#include <assert.h>


class Component 
{
public:

	Component(Entity * owner) :
		mOwner(owner) {}
	~Component() {}

	Entity * mOwner;

	virtual void Run(float deltaTime) = 0;
};

// *************************************************
//
// *************************************************

class C_Renderable : public Component, public IMessageReceiver
{
public:

	C_Renderable(Entity * owner, vec2 pos, vec2 size, const char * image) :
		Component     (owner),
		mPos         (pos),
		mSize        (size),
		mImage       (image),
	    mInitialized (false) {}

	ISprite * GetSprite () const { return m_pSprite; }
	virtual void Run   (float deltaTime) {}
	
	void Init()
	{
		assert(g_pGame);
		if (!mInitialized)
		{
			m_pSprite = g_pGraphicsEngine->RequireSprite(mPos, mSize, mImage);
		}
	}


	vec2 GetPos() const
	{
		return mPos;
	}

	void SetPos(float x, float y)
	{
		mPos.x = x;
		mPos.y = y;
		m_pSprite->SetPos(x, y);
	}

	vec2 GetSize() const
	{
		return mSize;
	}

	void SetSize(float x, float y)
	{
		mSize.x = x;
		mSize.y = y;
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
				SetPos(mPos.x + apm->GetX(), mPos.y + apm->GetY());
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
						rpm->SetX(mPos.x);
						rpm->SetY(mPos.y);
						rpm->SetProcessed(true);
					}
					else
					{
						RequireSizeMessage * rsm = dynamic_cast<RequireSizeMessage *>(&message);
						if (rsm)
						{
							rsm->SetX(mSize.x);
							rsm->SetY(mSize.y);
							rsm->SetProcessed(true);
						}
						else
						{
							ChangeSpriteMessage * csm = dynamic_cast<ChangeSpriteMessage *>(&message);
							if (csm)
							{
								const char * requiredTexture = csm->GetImage();
								if (strcmp(requiredTexture, mImage)) {
									// TODO cuidado con esto
									mImage = requiredTexture;
									g_pGraphicsEngine->ReleaseSprite(m_pSprite);
									m_pSprite = g_pGraphicsEngine->RequireSprite(mPos, mSize, mImage);
								}
							}
						}
					}
				}
			}
		}
	}

private:
	
	vec2        mPos;
	vec2        mSize;
	const char *mImage;
	ISprite     *m_pSprite;
	bool        mInitialized;
};

// *************************************************
//
// *************************************************

class C_Movable : public Component, public IMessageReceiver, public IEventManager::IListener
{
public:

	C_Movable(Entity * owner, float speed) : 
		Component  (owner),
		mMovement { 0, 0 },
		mSpeed    (speed) {}

	virtual void Run(float deltaTime)
	{
		if (mMovement.x || mMovement.y)
		{
			mMovement.x *= deltaTime;
			mMovement.y *= deltaTime;

			mLastMovement.x = mMovement.x;
			mLastMovement.y = mMovement.y;

			AddPositionMessage apm(mMovement.x, mMovement.y);
			mOwner->ReceiveMessage(apm);

			mMovement.x = 0.f;
			mMovement.y = 0.f;
		}
		else {
			mLastMovement.x = 0.f;
			mLastMovement.y = 0.f;
		}
	}

	vec2 GetMovement() const
	{
		return mMovement;
	}

	void SetMovement(float x, float y)
	{
		mMovement.x = x;
		mMovement.y = y;
	}

	vec2 GetSpeed() const
	{
		return mMovement;
	}

	void SetSpeed(float speed)
	{
		mSpeed = speed;
	}

	void MoveUp()
	{
		mMovement.y = mSpeed;
	}

	void MoveDown()
	{
		mMovement.y = -mSpeed;
	}

	void MoveLeft()
	{
		mMovement.x = -mSpeed;
	}

	void MoveRight()
	{
		mMovement.x = mSpeed;
	}

	void ReceiveMessage(GameMessage &message)
	{
		RigidBodyCollisionMessage * wcm = dynamic_cast<RigidBodyCollisionMessage *>(&message);
		if (wcm)
		{
			if      (RigidBodyCollisionMessage::Type::CollisionX == wcm->GetType()) mOwner->ReceiveMessage(AddPositionMessage(-mLastMovement.x, 0.f));
			else if (RigidBodyCollisionMessage::Type::CollisionY == wcm->GetType()) mOwner->ReceiveMessage(AddPositionMessage(0.f, -mLastMovement.y));;
		}
		else
		{
			RequireMovementMessage * rmm = dynamic_cast<RequireMovementMessage *>(&message);
			if (rmm)
			{
				rmm->SetX(mLastMovement.x);
				rmm->SetY(mLastMovement.y);
				rmm->SetSpeed(mSpeed);
				rmm->SetProcessed(true);
			}
			else
			{
				SetMovementMessage * smm = dynamic_cast<SetMovementMessage *>(&message);
				if (smm)
				{
					mMovement.x = smm->GetX();
					mMovement.y = smm->GetY();
				}
				else
				{
					MoveUpMessage * mum = dynamic_cast<MoveUpMessage *>(&message);
					if (mum)
					{
						mMovement.y = mSpeed;
					}
					else
					{
						MoveDownMessage  * mdm = dynamic_cast<MoveDownMessage *>(&message);
						if (mdm)
						{
							mMovement.y = -mSpeed;
						}
						else
						{
							MoveLeftMessage  * mlm = dynamic_cast<MoveLeftMessage *>(&message);
							if (mlm)
							{
								mMovement.x = -mSpeed;
							}
							else
							{
								MoveRightMessage * mrm = dynamic_cast<MoveRightMessage *>(&message);
								if (mrm)
								{
									mMovement.x = mSpeed;
								}
							}
						}
					}	
				}
			}
		}
	}

	bool ProcessEvent(IEventManager::EM_Event event) 
	{

		switch (event) 
		{
			case IEventManager::EM_Event::MoveUp    : mOwner->ReceiveMessage(MoveUpMessage());    break;
			case IEventManager::EM_Event::MoveDown  : mOwner->ReceiveMessage(MoveDownMessage());  break;
			case IEventManager::EM_Event::MoveLeft  : mOwner->ReceiveMessage(MoveLeftMessage());  break;
			case IEventManager::EM_Event::MoveRight : mOwner->ReceiveMessage(MoveRightMessage()); break;
		}

		return true;
	}

private:

	vec2  mMovement;
	vec2  mLastMovement;
	float mSpeed;
};

// *************************************************
//
// *************************************************

class C_Player : public Component
{
public:

	C_Player(Entity * owner) :
		Component(owner) {}

	virtual void Run(float deltaTime)
	{
	}
};

// *************************************************
//
// *************************************************

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
		mPursuingSpeed (pursuingSpeed),
		mState         (CES_PATROLLING) {}

	virtual void Run(float deltaTime)
	{
		assert(g_pGame);		
		Entity * player = g_pGame->GetPlayer();

		RequirePositionMessage       positionMessage;
		RequireSizeMessage           sizeMessage;
		RequireMovementMessage       movementMessage;
		RequirePositionMessage       playerPositionMessage;
		RequireSizeMessage           playerSizeMessage;

		mOwner->ReceiveMessage(positionMessage);
		mOwner->ReceiveMessage(sizeMessage);
		mOwner->ReceiveMessage(movementMessage);
		player->ReceiveMessage(playerPositionMessage);
		player->ReceiveMessage(playerSizeMessage);

		assert(positionMessage.GetProcessed());
		assert(sizeMessage.GetProcessed());
		assert(movementMessage.GetProcessed());
		assert(playerPositionMessage.GetProcessed());
		assert(playerSizeMessage.GetProcessed());

		RequireRouteMessage routeMessage;
		mOwner->ReceiveMessage(routeMessage);

		vec2    pos                              = vmake(positionMessage.GetX()       , positionMessage.GetY());
		vec2    size                             = vmake(sizeMessage.GetX()           , sizeMessage.GetY());
		vec2    movement                         = vmake(movementMessage.GetX()       , movementMessage.GetY());
		float   speed                            = movementMessage.GetSpeed();
		vec2    playerPos                        = vmake(playerPositionMessage.GetX() , playerPositionMessage.GetY());
		vec2    playerSize                       = vmake(playerSizeMessage.GetX()     , playerSizeMessage.GetY());
		const   std::vector<vec2> * routePoints  = routeMessage.GetRoutePoints();
		uint8_t currentRoutePoint                = routeMessage.GetCurrentRoutePoint();

		if (CES_PATROLLING == mState && routePoints && !routePoints->empty())
		{
			vec2 nextRoutePoint = (*routePoints)[currentRoutePoint];

			vec2 direction;

			float angle = atan2(nextRoutePoint.y - pos.y, nextRoutePoint.x - pos.x);
			direction = vunit(angle);
			
			SetMovementMessage smm(speed * direction.x, speed * direction.y);
			mOwner->ReceiveMessage(smm);

			mOwner->ReceiveMessage(IncreaseCurrentRoutePointMessage(pos.x, pos.y));

			bool enemy_sights = false;
			if (movement.x < 0 && playerPos.x < pos.x)
			{
				if ((playerPos.y <= (pos.y + size.y) && playerPos.y >= pos.y)
					|| ((playerPos.y + playerSize.y) <= (pos.y + size.y) && (playerPos.y + playerSize.y) >= pos.y))
				{
					enemy_sights = true;
					OutputDebugString("Enemy sights player\n");
					mState = CES_PURSUING;
				}
			}
			else if (movement.x > 0 && playerPos.x > pos.x)
			{
				if ((playerPos.y <= (pos.y + size.y) && playerPos.y >= pos.y)
					|| ((playerPos.y + playerSize.y) <= (pos.y + size.y) && (playerPos.y + playerSize.y) >= pos.y))
				{
					enemy_sights = true;
					OutputDebugString("Enemy sights player\n");
					mState = CES_PURSUING;
				}
			}
		} 
		else
		{
			OutputDebugString("Enemy pursuing player\n");

			float angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
			vec2 direction = vunit(angle);
			SetMovementMessage smm(mPursuingSpeed * direction.x, mPursuingSpeed * direction.y);
			mOwner->ReceiveMessage(smm);
		}

		const char * required_texture = ((movement.x < 0) ? "../data/soldier_left.png" : "../data/soldier_right.png");
		ChangeSpriteMessage csm(required_texture);
		mOwner->ReceiveMessage(csm);
	}

	void ReceiveMessage(GameMessage &message)
	{
		RequirePursuingSpeedMessage * rpsm = dynamic_cast<RequirePursuingSpeedMessage *>(&message);
		if (rpsm)
		{
			rpsm->SetPursuingSpeed(mPursuingSpeed);
			rpsm->SetProcessed(true);
		}
	}

private:

	State mState;
	float mPursuingSpeed;
};

// *************************************************
//
// *************************************************

class C_RoutePath : public Component, public IMessageReceiver
{
public:
	
	C_RoutePath(Entity * owner) :
		Component           (owner),
		mCurrentRoutePoint (0) {}

	virtual void Run(float deltaTime)
	{
		
	}

	void AddRoutePoint(float x, float y)
	{
		mRoutePoints.push_back(vmake(x, y));
	}

	void RemoveRoutePoint(float x, float y)
	{
		for (auto it = mRoutePoints.begin(); it != mRoutePoints.end(); ) 
		{
			if ((*it).x == x && (*it).y == y) 
			{
				it = mRoutePoints.erase(it);
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
		mRoutePoints.clear();
	}

	void ReceiveMessage(GameMessage &message)
	{
		RequireRouteMessage * rrm = dynamic_cast<RequireRouteMessage *>(&message);
		if (rrm)
		{
			rrm->SetCurrentRoutePoint(mCurrentRoutePoint);
			rrm->SetRoutePoints(&mRoutePoints);
		}
		else
		{
			IncreaseCurrentRoutePointMessage * icrpm = dynamic_cast<IncreaseCurrentRoutePointMessage *>(&message);
			if (icrpm)
			{ 
				if (abs(mRoutePoints[mCurrentRoutePoint].x - icrpm->GetCurrentX()) < 2 && abs(mRoutePoints[mCurrentRoutePoint].y - icrpm->GetCurrentY()) < 2)
					++mCurrentRoutePoint;
				if(mCurrentRoutePoint >= mRoutePoints.size())
					mCurrentRoutePoint = 0;
			}
		}
	}

private:

	std::vector<vec2> mRoutePoints;
	uint8_t           mCurrentRoutePoint;
};

// *************************************************
//
// *************************************************

class C_RigidBody : public Component
{
public:

	C_RigidBody(Entity * owner) :
		Component(owner) {}

	virtual void Run(float deltaTime)
	{
	}
};

// *************************************************
//
// *************************************************

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

