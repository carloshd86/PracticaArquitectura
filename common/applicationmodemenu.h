#ifndef __APPLICATION_MODE_MENU_H__
#define __APPLICATION_MODE_MENU_H__


#include "applicationmode.h"
#include "navigationcontainer.h"
#include "button.h"
#include <map>
#include <functional>


class ApplicationModeMenu : public ApplicationMode, public IEventManager::IListener, public Button::IListener
{
public:

	ApplicationModeMenu();

	IdMode GetId           ();
	void   Activate        ();
	void   Deactivate      ();
	void   ProcessInput    ();
	void   Run             (float deltaTime);
	void   Render          ();

	bool ProcessEvent (IEventManager::EM_Event event);
	void OnClick      (Button * button);

private:

	bool                                      mWaitEscapeRelease;
	NavigationContainer                       mContainer;
	std::map<Button *, std::function<void()>> mButtonMap;

	void StartLevel1();
	void StartLevel2();
	void StartLevel3();
};

#endif