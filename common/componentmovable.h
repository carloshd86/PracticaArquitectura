#ifndef __COMPONENT_MOVABLE_H__
#define __COMPONENT_MOVABLE_H__


#include "component.h"


class C_Movable : public Component, public IMessageReceiver
{
public:

	C_Movable(Entity * owner, float speed) : 
		Component  (owner),
		mMovement { 0, 0 },
		mSpeed    (speed) {}

	virtual ~C_Movable()
	{
		End();
	}

	virtual void Init()
	{
		if (!mInitialized)
		{ 
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RigidbodyCollision , std::bind(&C_Movable::OnRigidbodyCollision , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RequireMovement    , std::bind(&C_Movable::OnRequireMovement    , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::SetMovement        , std::bind(&C_Movable::OnSetMovement        , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::MoveUp             , std::bind(&C_Movable::OnMoveUp             , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::MoveDown           , std::bind(&C_Movable::OnMoveDown           , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::MoveLeft           , std::bind(&C_Movable::OnMoveLeft           , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::MoveRight          , std::bind(&C_Movable::OnMoveRight          , this, std::placeholders::_1)));

			mInitialized = true;
		}
	}

	virtual void End()
	{
		if (mInitialized)
		{ 
			mMessageCallbacks.clear();

			mInitialized = false;
		}
	}

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
		GameMessage::GM_Type messageType = message.GetType();
		if (mMessageCallbacks.end() != mMessageCallbacks.find(messageType))
			mMessageCallbacks[messageType](message);
	}

private:

	vec2               mMovement;
	vec2               mLastMovement;
	float              mSpeed;
	MessageCallbackMap mMessageCallbacks;
	bool               mInitialized;

	void OnRigidbodyCollision(GameMessage& message)
	{
		RigidbodyCollisionMessage * wcm = dynamic_cast<RigidbodyCollisionMessage *>(&message);
		if (wcm)
		{
			RigidbodyCollisionMessage::RGBM_Type collisionType = wcm->GetCollisionType();
			if      (RigidbodyCollisionMessage::RGBM_Type::CollisionX == collisionType) mOwner->ReceiveMessage(AddPositionMessage(-mLastMovement.x, 0.f));
			else if (RigidbodyCollisionMessage::RGBM_Type::CollisionY == collisionType) mOwner->ReceiveMessage(AddPositionMessage(0.f, -mLastMovement.y));;
		}
	}

	void OnRequireMovement(GameMessage& message)
	{
		RequireMovementMessage * rmm = dynamic_cast<RequireMovementMessage *>(&message);
		if (rmm)
		{
			rmm->SetX(mLastMovement.x);
			rmm->SetY(mLastMovement.y);
			rmm->SetSpeed(mSpeed);
			rmm->SetProcessed(true);
		}
	}

	void OnSetMovement(GameMessage& message)
	{
		SetMovementMessage * smm = dynamic_cast<SetMovementMessage *>(&message);
		if (smm)
		{
			mMovement.x = smm->GetX();
			mMovement.y = smm->GetY();
		}
	}

	void OnMoveUp(GameMessage& message)
	{
		MoveUpMessage * mum = dynamic_cast<MoveUpMessage *>(&message);
		if (mum)
		{
			mMovement.y = mSpeed;
		}
	}

	void OnMoveDown(GameMessage& message)
	{
		MoveDownMessage  * mdm = dynamic_cast<MoveDownMessage *>(&message);
		if (mdm)
		{
			mMovement.y = -mSpeed;
		}
	}

	void OnMoveLeft(GameMessage& message)
	{
		MoveLeftMessage  * mlm = dynamic_cast<MoveLeftMessage *>(&message);
		if (mlm)
		{
			mMovement.x = -mSpeed;
		}
	}

	void OnMoveRight(GameMessage& message)
	{
		MoveRightMessage * mrm = dynamic_cast<MoveRightMessage *>(&message);
		if (mrm)
		{
			mMovement.x = mSpeed;
		}
	}
};

#endif
