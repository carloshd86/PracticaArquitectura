#include "sysinputmanager.h"
#include "stdafx.h"
#include "globals.h"
#include "sys.h"
#include <algorithm>
#include <utility>


SysInputManager * SysInputManager::mInstance;

// *************************************************
//
// *************************************************

SysInputManager::SysInputManager() :
	mInitialized (false) {}

// *************************************************
//
// *************************************************

SysInputManager::~SysInputManager() {
	if (mInitialized)
		End();
}

// *************************************************
//
// *************************************************

SysInputManager * SysInputManager::Instance() {
	if (!mInstance) {
		mInstance = new SysInputManager();
		mInstance->Init();
	}

	return mInstance;
}

// *************************************************
//
// *************************************************

IEventManager::EM_Err SysInputManager::Init() {

	if (mInitialized)
		return OK;

	mInitialized = true;
	return OK;
}

// *************************************************
//
// *************************************************

IEventManager::EM_Err SysInputManager::End() {
	return OK;
}

// *************************************************
//
// ************************************************

void SysInputManager::UpdateEvents() {
	for (auto it = mListeners.begin(); it != mListeners.end(); ++it) {
		int key = GetSysKeyFromEvent(it->first);
		if(SYS_KeyPressed(key))
			SendEvent(it->first, it->second);
	}
}

// *************************************************
//
// *************************************************

IEventManager::EM_Err SysInputManager::Register(IListener * listener, EM_Event e, int priority) {

	mListeners[e].insert(std::pair<int, IListener *>(priority, listener));

	return OK;
}

// *************************************************
//
// *************************************************

IEventManager::EM_Err SysInputManager::Unregister(IListener * listener) {

	for (auto it = mListeners.begin(); it != mListeners.end(); ++it) {
		EM_Event event = it->first;
		RemoveListenerForEvent(listener, event);
	}

	return OK;
}

// *************************************************
//
// *************************************************

SysInputManager::ListenerMap& SysInputManager::GetListenerMap() {
	return mListeners;
}

// *************************************************
//
// *************************************************

void SysInputManager::RemoveListenerForEvent(IListener * listener, EM_Event e) {

	for(auto prioritiesIt = mListeners[e].begin(); prioritiesIt != mListeners[e].end(); ++prioritiesIt) {
		if (prioritiesIt->second == listener) {
			prioritiesIt = mListeners[e].erase(prioritiesIt);
			break;
		}
	}

}

// *************************************************
//
// *************************************************

void SysInputManager::SendEvent(EM_Event event, const EventsMultiMap& eventsMultiMap) {

	if (eventsMultiMap.size()) {
		for (auto prioritiesIt = eventsMultiMap.begin(); prioritiesIt != eventsMultiMap.end(); ++prioritiesIt) {
			prioritiesIt->second->ProcessEvent(event);
		}
	}
}

// *************************************************
//
// *************************************************�

int SysInputManager::GetSysKeyFromEvent(EM_Event event) {
	int key = -1;
	switch (event) {
		case EM_Event::MoveUp      : key = SYS_KEY_UP;     break;
		case EM_Event::MoveDown    : key = SYS_KEY_DOWN;   break;
		case EM_Event::MoveLeft    : key = SYS_KEY_LEFT;   break;
		case EM_Event::MoveRight   : key = SYS_KEY_RIGHT;  break;
		case EM_Event::PressEscape : key = VK_ESCAPE;      break;
	}

	return key;
}