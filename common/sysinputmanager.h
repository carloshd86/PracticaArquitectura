#ifndef __SYS_INPUT_MANAGER_H__
#define __SYS_INPUT_MANAGER_H__

#include "eventmanager.h"
#include <map>
#include <vector>

class SysInputManager : public IEventManager {
	
public:

	typedef std::multimap<int, IListener *> EventsMultiMap;
	typedef std::map<EM_Event, EventsMultiMap>   ListenerMap;

	static SysInputManager * Instance();
	~SysInputManager();

	EM_Err Init();
	EM_Err End();

	void   UpdateEvents ();
	EM_Err Register     (IListener * listener, EM_Event e, int priority);
	EM_Err Unregister   (IListener * listener);

	ListenerMap& GetListenerMap();

private:
	SysInputManager();

	struct PressStatus
	{
		PressStatus() : currentPress(false), lastPress(false) {}

		bool currentPress;
		bool lastPress;
	};

	static SysInputManager *mInstance;

	static const int NO_KEY_PRESSED = -1;

	ListenerMap                mListeners;
	std::map<int, PressStatus> mPressStatusMap;
	bool                       mInitialized;

	void SendEvent              (EM_Event event, const EventsMultiMap& eventsMultiMap);
	void RemoveListenerForEvent (IListener * listener, EM_Event e);
	int  GetSysKeyFromEvent     (EM_Event event);
};

#endif