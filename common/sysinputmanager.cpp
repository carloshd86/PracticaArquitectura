#include "stdafx.h"
#include "sysinputmanager.h"
#include "globals.h"
#include "sys.h"
#include "memorycontrol.h"
#include <algorithm>
#include <utility>


SysInputManager * SysInputManager::mInstance;

// *************************************************
//
// *************************************************

SysInputManager::SysInputManager() :
	mInitialized   (false) {}

// *************************************************
//
// *************************************************

SysInputManager::~SysInputManager() 
{
	if (mInitialized)
		End();
}

// *************************************************
//
// *************************************************

SysInputManager * SysInputManager::Instance() 
{
	if (!mInstance) 
	{
		mInstance = GAME_NEW(SysInputManager, ());
		mInstance->Init();
	}

	return mInstance;
}

// *************************************************
//
// *************************************************

IEventManager::EM_Err SysInputManager::Init() 
{
	if (mInitialized)
		return OK;

	mInitialized = true;
	return OK;
}

// *************************************************
//
// *************************************************

IEventManager::EM_Err SysInputManager::End() 
{
	return OK;
}

// *************************************************
//
// ************************************************

void SysInputManager::UpdateEvents() 
{
	for (auto it = mListeners.begin(); it != mListeners.end(); ++it) 
	{
		int eventKey = GetSysKeyFromEvent(it->first);
		if (SYS_KeyPressed(eventKey))
			mPressStatusMap[eventKey].currentPress = true;

		switch (it->first)
		{
			case IEventManager::EM_Event::SinglePressEnter  :
			case IEventManager::EM_Event::SinglePressEscape :
			case IEventManager::EM_Event::SinglePressUp     :
			case IEventManager::EM_Event::SinglePressDown   :
			case IEventManager::EM_Event::SinglePressLeft   :
			case IEventManager::EM_Event::SinglePressRight  : 
			{ 
				if (!mPressStatusMap[eventKey].lastPress && mPressStatusMap[eventKey].currentPress) 
					SendEvent(it->first, it->second); 
				break; 
			}

			case IEventManager::EM_Event::MoveUp   :
			case IEventManager::EM_Event::MoveDown :
			case IEventManager::EM_Event::MoveLeft :
			case IEventManager::EM_Event::MoveRight: 
			{ 
				if (mPressStatusMap[eventKey].currentPress) 
					SendEvent(it->first, it->second); 
				break; 
			}
		}
	}
	for (auto& pressStatus : mPressStatusMap)
	{
		pressStatus.second.lastPress    = pressStatus.second.currentPress;
		pressStatus.second.currentPress = false;
	}
		
}

// *************************************************
//
// *************************************************

IEventManager::EM_Err SysInputManager::Register(IListener * listener, EM_Event e, int priority) 
{
	mListeners[e].insert(std::pair<int, IListener *>(priority, listener));
	int eventKey = GetSysKeyFromEvent(e);
	if (mPressStatusMap.find(eventKey) == mPressStatusMap.end())
		mPressStatusMap.insert(std::pair<int, PressStatus>(eventKey, PressStatus()));

	return OK;
}

// *************************************************
//
// *************************************************

IEventManager::EM_Err SysInputManager::Unregister(IListener * listener) 
{
	auto it = mListeners.begin();
	while (it != mListeners.end()) 
	{
		EM_Event event = it->first;
		RemoveListenerForEvent(listener, event);

		if (mListeners[event].empty())
			it = mListeners.erase(it);
		else
			++it;
	}

	return OK;
}

// *************************************************
//
// *************************************************

SysInputManager::ListenerMap& SysInputManager::GetListenerMap() 
{
	return mListeners;
}

// *************************************************
//
// *************************************************

void SysInputManager::RemoveListenerForEvent(IListener * listener, EM_Event e) 
{
	for(auto prioritiesIt = mListeners[e].begin(); prioritiesIt != mListeners[e].end(); ++prioritiesIt) 
	{
		if (prioritiesIt->second == listener) 
		{
			prioritiesIt = mListeners[e].erase(prioritiesIt);
			break;
		}
	}

}

// *************************************************
//
// *************************************************

void SysInputManager::SendEvent(EM_Event event, const EventsMultiMap& eventsMultiMap) 
{
	if (eventsMultiMap.size()) 
	{
		for (auto prioritiesIt = eventsMultiMap.begin(); prioritiesIt != eventsMultiMap.end(); ++prioritiesIt) 
		{
			prioritiesIt->second->ProcessEvent(event);
		}
	}
}

// *************************************************
//
// *************************************************ç

int SysInputManager::GetSysKeyFromEvent(EM_Event event) 
{
	int key = NO_KEY_PRESSED;
	switch (event) 
	{
		case EM_Event::MoveUp            : 
		case EM_Event::SinglePressUp     : key = SYS_KEY_UP;     break;

		case EM_Event::MoveDown          :
		case EM_Event::SinglePressDown   : key = SYS_KEY_DOWN;   break;

		case EM_Event::MoveLeft          :
		case EM_Event::SinglePressLeft   : key = SYS_KEY_LEFT;   break;

		case EM_Event::MoveRight         :
		case EM_Event::SinglePressRight  : key = SYS_KEY_RIGHT;  break;

		case EM_Event::SinglePressEscape : key = VK_ESCAPE;      break;
		case EM_Event::SinglePressEnter  : key = VK_RETURN;      break;
	}

	return key;
}