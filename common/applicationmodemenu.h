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
	~ApplicationModeMenu();

	IdMode GetId           ();
	void   Activate        ();
	void   Deactivate      ();
	void   ProcessInput    ();
	void   Run             (float deltaTime);
	void   Render          ();
	void   ChangeLanguage  (Properties::P_Language language);

	bool ProcessEvent (IEventManager::EM_Event event);
	void OnClick      (Button * button);


private:

	NavigationContainer                      *mCurrentContainer;
	std::vector<NavigationContainer *>        mContainers;
	std::map<Button *, std::function<void()>> mButtonMap;
	Properties                               *m_pProperties;

	void StartLevel1 ();
	void StartLevel2 ();
	void StartLevel3 ();
	void OpenMenu    (int index);
};

#endif