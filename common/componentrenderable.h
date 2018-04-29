#ifndef __COMPONENT_RENDERABLE_H__
#define __COMPONENT_RENDERABLE_H__


#include "component.h"


class C_Renderable : public Component, public IMessageReceiver
{
public:

	C_Renderable(Entity * owner, vec2 pos, vec2 size, const char * image) :
		Component    (owner),
		mPos         (pos),
		mSize        (size),
		mImage       (image),
		mInitialized (false) {}

	virtual ~C_Renderable()
	{
		End();
	}

	virtual void Init()
	{
		if (!mInitialized)
		{
			assert(g_pGraphicsEngine);
			m_pSprite = g_pGraphicsEngine->RequireSprite(mPos, mSize, mImage.c_str());

			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::SetPosition           , std::bind(&C_Renderable::OnSetPosition     , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::AddPosition           , std::bind(&C_Renderable::OnAddPosition     , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::SetSize               , std::bind(&C_Renderable::OnSetSize         , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RequireRenderPosition , std::bind(&C_Renderable::OnRequirePosition , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RequireRenderSize     , std::bind(&C_Renderable::OnRequireSize     , this, std::placeholders::_1)));
			mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::ChangeSprite          , std::bind(&C_Renderable::OnChangeSprite    , this, std::placeholders::_1)));

			mInitialized = true;
		}
	}

	virtual void End()
	{
		if (mInitialized)
		{
			assert(g_pGraphicsEngine);
			g_pGraphicsEngine->ReleaseSprite(m_pSprite);

			mMessageCallbacks.clear();

			mInitialized = false;
		}
	}

	ISprite * GetSprite () const { return m_pSprite; }
	virtual void Run   (float deltaTime) {}

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
		GameMessage::GM_Type messageType = message.GetType();
		if (mMessageCallbacks.end() != mMessageCallbacks.find(messageType))
			mMessageCallbacks[messageType](message);
	}

private:

	vec2               mPos;
	vec2               mSize;
	std::string        mImage;
	ISprite           *m_pSprite;
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
		RequireRenderPositionMessage * rpm = dynamic_cast<RequireRenderPositionMessage *>(&message);
		if (rpm)
		{
			rpm->SetX(mPos.x);
			rpm->SetY(mPos.y);
			rpm->SetProcessed(true);
		}
	}

	void OnRequireSize(GameMessage& message)
	{
		RequireRenderSizeMessage * rsm = dynamic_cast<RequireRenderSizeMessage *>(&message);
		if (rsm)
		{
			rsm->SetX(mSize.x);
			rsm->SetY(mSize.y);
			rsm->SetProcessed(true);
		}
	}

	void OnChangeSprite(GameMessage& message)
	{
		ChangeSpriteMessage * csm = dynamic_cast<ChangeSpriteMessage *>(&message);
		if (csm)
		{
			const char * requiredTexture = csm->GetImage();
			if (strcmp(requiredTexture, mImage.c_str())) 
			{
				mImage = requiredTexture;
				g_pGraphicsEngine->ReleaseSprite(m_pSprite);
				m_pSprite = g_pGraphicsEngine->RequireSprite(mPos, mSize, mImage.c_str());
			}
		}
	}
};


#endif
