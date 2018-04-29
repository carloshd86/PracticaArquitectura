#ifndef __COMPONENT_PLAYER_H__
#define __COMPONENT_PLAYER_H__


#include "component.h"
#include "asserts.h"


class C_Player : public Component, public IEventManager::IListener
{
public:

	C_Player(Entity * owner) : 
		Component    (owner),
		mInitialized (false) {}

	virtual ~C_Player()
	{
		End();
	}

	virtual void Init()
	{
		if (!mInitialized)
		{
			GAME_ASSERT(g_pEventManager);
			g_pEventManager->Register(this, IEventManager::EM_Event::MoveUp    , 0);
			g_pEventManager->Register(this, IEventManager::EM_Event::MoveDown  , 0);
			g_pEventManager->Register(this, IEventManager::EM_Event::MoveLeft  , 0);
			g_pEventManager->Register(this, IEventManager::EM_Event::MoveRight , 0);

			mInitialized = true;
		}
	}

	virtual void End()
	{
		if (mInitialized)
		{
			GAME_ASSERT(g_pEventManager);
			g_pEventManager->Unregister(this);

			mInitialized = false;
		}
	}

	virtual void Run(float deltaTime)
	{
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

	bool mInitialized;
};

#endif
