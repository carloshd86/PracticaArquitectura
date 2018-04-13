#ifndef __APPLICATION_MODE_GAME_H__
#define __APPLICATION_MODE_GAME_H__


#include "applicationmode.h"
#include "navigationcontainer.h"
#include "button.h"
#include <map>
#include <functional>


class ApplicationModeGame : public ApplicationMode, public IEventManager::IListener, public Button::IListener
{
public:

	ApplicationModeGame();
	~ApplicationModeGame();

	IdMode GetId           ();
	void   Activate        ();
	void   Deactivate      ();
	void   ProcessInput    ();
	void   Run             (float deltaTime);
	void   Render          ();
	void   ChangeLanguage  (Properties::P_Language lang);

	bool ProcessEvent (IEventManager::EM_Event event);
	void OnClick      (Button * button);

private:

	bool                                      mInGameMenuActive;
	NavigationContainer                      *mCurrentContainer;
	std::vector<NavigationContainer *>        mContainers;
	std::map<Button *, std::function<void()>> mButtonMap;
	Properties                               *m_pProperties;

	void ResumeGame ();
	void QuitGame   ();
	void OpenMenu   (int index);
};

#endif