#ifndef __APPLICATION_MODE_MENU_H__
#define __APPLICATION_MODE_MENU_H__


#include "applicationmode.h"
#include "game.h"
#include "navigationcontainer.h"
#include "button.h"
#include "checkbox.h"
#include "slider.h"
#include <map>
#include <functional>

class ApplicationModeMenu : public ApplicationMode, public IEventManager::IListener, public Button::IListener, public Checkbox::IListener, public Slider::IListener
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
	void   ChangeLanguage  (Properties::P_Language lang);

	bool ProcessEvent        (IEventManager::EM_Event event);
	void OnClick             (Button * button);
	void OnClick             (Checkbox * checkbox);
	void OnPercentageChanged (Slider* slider);


private:

	NavigationContainer                       *mCurrentContainer;
	std::vector<NavigationContainer *>         mContainers;
	std::map<Control *, std::function<void()>> mControlMap;
	Properties                                *m_pProperties;
	unsigned int                               mMusicId;
	ISprite                                   *m_pSprite;

	Button   * InitButton   (std::function<void()> clickFunction, float x, float y, float width, float height, Container * parent, const char * textKey, float rOn = 1.f, float gOn = 1.f, float bOn = 0.f, float rOff = 1.f, float gOff = 1.f, float bOff = 1.f);
	Checkbox * InitCheckbox (float x, float y, float width, float height, Container * parent, const char * textKey, bool checked, float rOn = 1.f, float gOn = 1.f, float bOn = 0.f, float rOff = 1.f, float gOff = 1.f, float bOff = 1.f);
	Slider   * InitSlider   (float x, float y, float width, float height, Container * parent, const char * textKey, float rOn = 1.f, float gOn = 1.f, float bOn = 0.f, float rOff = 1.f, float gOff = 1.f, float bOff = 1.f);

	void StartLevel           (Game::GameLevel level);
	void OpenMenu             (int index);
	void ChangeActivatedAudio (Checkbox * checkbox);
	void ChangeVolume         (Slider* slider);
	void QuitGame             ();
};

#endif