#ifndef __COMPONENT_RIGIDBODY_H__
#define __COMPONENT_RIGIDBODY_H__


#include "component.h"


class C_Rigidbody : public Component, public IMessageReceiver
{
public:

	C_Rigidbody(Entity * owner, vec2 pos, vec2 size) :
		Component    (owner),
		mPos         (pos),
		mSize        (size),
		mInitialized (false) {}

	virtual ~C_Rigidbody()
	{
		End();
	}

	virtual void Init()
	{
		if (!mInitialized)
		{
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::SetPosition              , std::bind(&C_Rigidbody::OnSetPosition     , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::AddPosition              , std::bind(&C_Rigidbody::OnAddPosition     , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::SetSize                  , std::bind(&C_Rigidbody::OnSetSize         , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RequireRigidbodyPosition , std::bind(&C_Rigidbody::OnRequirePosition , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RequireRigidbodySize     , std::bind(&C_Rigidbody::OnRequireSize     , this, std::placeholders::_1)));

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

	virtual void Run   (float deltaTime) {}

	vec2 GetPos() const
	{
		return mPos;
	}

	void SetPos(float x, float y)
	{
		mPos.x = x;
		mPos.y = y;
	}

	vec2 GetSize() const
	{
		return mSize;
	}

	void SetSize(float x, float y)
	{
		mSize.x = x;
		mSize.y = y;
	}

	void ReceiveMessage(GameMessage &message)
	{
		GameMessage::GM_Type messageType = message.GetType();
		if (mMessageCallbacks.end() != mMessageCallbacks.find(messageType))
			mMessageCallbacks[messageType](message);
	}

private:

	vec2               mPos;
	vec2               mSize;
	bool               mInitialized;
	MessageCallbackMap mMessageCallbacks;

	void OnSetPosition(GameMessage& message)
	{
		SetPositionMessage * spm = dynamic_cast<SetPositionMessage *>(&message);
		if (spm)
		{
			SetPos(spm->GetX(), spm->GetY());
		}
	}

	void OnAddPosition(GameMessage& message)
	{
		AddPositionMessage * apm = dynamic_cast<AddPositionMessage *>(&message);
		if (apm)
		{
			SetPos(mPos.x + apm->GetX(), mPos.y + apm->GetY());
		}
	}

	void OnSetSize(GameMessage& message)
	{
		SetSizeMessage * ssm = dynamic_cast<SetSizeMessage *>(&message);
		if (ssm)
		{
			SetSize(ssm->GetX(), ssm->GetY());
		}
	}

	void OnRequirePosition(GameMessage& message)
	{
		RequireRigidbodyPositionMessage * rpm = dynamic_cast<RequireRigidbodyPositionMessage *>(&message);
		if (rpm)
		{
			rpm->SetX(mPos.x);
			rpm->SetY(mPos.y);
			rpm->SetProcessed(true);
		}
	}

	void OnRequireSize(GameMessage& message)
	{
		RequireRigidbodySizeMessage * rsm = dynamic_cast<RequireRigidbodySizeMessage *>(&message);
		if (rsm)
		{
			rsm->SetX(mSize.x);
			rsm->SetY(mSize.y);
			rsm->SetProcessed(true);
		}
	}

};


#endif
