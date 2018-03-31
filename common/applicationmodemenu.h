#ifndef __APPLICATION_MODE_MENU_H__
#define __APPLICATION_MODE_MENU_H__


#include "applicationmode.h"


class ApplicationModeMenu : public ApplicationMode, public IEventManager::IListener
{
public:

	ApplicationModeMenu();

	IdMode GetId           ();
	void   Activate        ();
	void   Deactivate      ();
	void   ProcessInput    ();
	void   Run             (float deltaTime);
	void   Render          ();

	bool ProcessEvent(IEventManager::EM_Event event);

private:

	bool m_waitEscapeRelease;
};

#endif