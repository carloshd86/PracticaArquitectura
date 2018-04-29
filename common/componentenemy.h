#ifndef __COMPONENT_ENEMY_H__
#define __COMPONENT_ENEMY_H__


#include "component.h"


class C_Enemy : public Component, public IMessageReceiver
{

public:

	enum CE_State
	{
		Patrolling,
		Pursuing
	};

	C_Enemy(Entity * owner, float pursuingSpeed, const char * imgLeft, const char * imgRight, const char * imgAlertLeft, const char * imgAlertRight) :
		Component      (owner),
		mPursuingSpeed (pursuingSpeed),
		mImgLeft       (imgLeft),
		mImgRight      (imgRight),
		mImgAlertLeft  (imgAlertLeft),
		mImgAlertRight (imgAlertRight),
		mState         (Patrolling) {}

	virtual ~C_Enemy()
	{
		End();
	}

	virtual void Init()
	{
		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RequirePursuingSpeed, std::bind(&C_Enemy::OnRequirePursuingSpeed, this, std::placeholders::_1)));

		mInitialized = true;
	}

	virtual void End()
	{
		mMessageCallbacks.clear();

		mInitialized = false;
	}

	virtual void Run(float deltaTime)
	{
		assert(g_pGame);		
		Entity * player = g_pGame->GetPlayer();

		RequireRenderPositionMessage positionMessage;
		RequireRenderSizeMessage     sizeMessage;
		RequireMovementMessage       movementMessage;
		RequireRenderPositionMessage playerPositionMessage;
		RequireRenderSizeMessage     playerSizeMessage;

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

		if (Patrolling == mState && routePoints && !routePoints->empty())
		{
			vec2 nextRoutePoint = (*routePoints)[currentRoutePoint];

			vec2 direction;

			float angle = atan2(nextRoutePoint.y - pos.y, nextRoutePoint.x - pos.x);
			direction = vunit(angle);

			SetMovementMessage smm(speed * direction.x, speed * direction.y);
			mOwner->ReceiveMessage(smm);

			mOwner->ReceiveMessage(IncreaseCurrentRoutePointMessage(pos.x, pos.y));

			bool enemy_sights = false;
			float playerAngle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
			vec2 playerDirector = vunit(playerAngle);
			float sightDirection = movement.x >= 0 ? 1.f : -1.f;
			vec2 visionLine = vmake(sightDirection, 0.f);
			float dot = vdot(playerDirector, visionLine);
			if (dot > 0.99)
			{
				enemy_sights = true;
				OutputDebugString("Enemy sights player\n");
				mState = Pursuing;
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

		const char * required_texture = nullptr;
		if (Patrolling == mState) required_texture = (movement.x < 0) ? mImgLeft.c_str()      : mImgRight.c_str();
		else                      required_texture = (movement.x < 0) ? mImgAlertLeft.c_str() : mImgAlertRight.c_str();

		if (required_texture)
		{
			ChangeSpriteMessage csm(required_texture);
			mOwner->ReceiveMessage(csm);
		}
	}

	void ReceiveMessage(GameMessage &message)
	{
		GameMessage::GM_Type messageType = message.GetType();
		if (mMessageCallbacks.end() != mMessageCallbacks.find(messageType))
			mMessageCallbacks[messageType](message);
	}

private:

	CE_State           mState;
	float              mPursuingSpeed;
	bool               mInitialized;
	MessageCallbackMap mMessageCallbacks;
	std::string        mImgLeft;
	std::string        mImgRight;
	std::string        mImgAlertLeft;
	std::string        mImgAlertRight;

	void OnRequirePursuingSpeed(GameMessage& message)
	{
		RequirePursuingSpeedMessage * rpsm = dynamic_cast<RequirePursuingSpeedMessage *>(&message);
		if (rpsm)
		{
			rpsm->SetPursuingSpeed(mPursuingSpeed);
			rpsm->SetProcessed(true);
		}
	}

};

#endif
