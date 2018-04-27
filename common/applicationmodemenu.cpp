#include "stdafx.h"
#include "globals.h"
#include "applicationmodemenu.h"
#include "applicationmanager.h"
#include "sys.h"
#include "core.h"
#include "font.h"
#include "button.h"
#include <assert.h>


ApplicationModeMenu::ApplicationModeMenu () :
	mCurrentContainer (nullptr) {}

// *************************************************
//
// *************************************************

ApplicationModeMenu::~ApplicationModeMenu()
{
	auto it = mContainers.begin();
	while (it != mContainers.end()) 
	{
		delete (*it);
		it = mContainers.erase(it);
	}
}

// *************************************************
//
// *************************************************

IdMode ApplicationModeMenu::GetId()
{
	return AM_Menu;
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::Activate()
{
	assert(g_pEventManager);

	m_pProperties = Properties::Instance("messages", g_pApplicationManager->GetLang());
	assert(m_pProperties);

	g_pEventManager->Register(this, IEventManager::EM_Event::SinglePressUp     , 0);
	g_pEventManager->Register(this, IEventManager::EM_Event::SinglePressDown   , 0);
	g_pEventManager->Register(this, IEventManager::EM_Event::SinglePressEscape , 0);


	// Container main menu
	NavigationContainer * mainContainer = new NavigationContainer();
	mCurrentContainer = mainContainer;

	Button * levelsButton  = InitButton(std::bind(&ApplicationModeMenu::OpenMenu, this, 1) , SCR_HEIGHT/4.f, 330.f, 200.f, SCR_HEIGHT*0.25f, mainContainer, "main_menu.levels.text");
	Button * optionsButton = InitButton(std::bind(&ApplicationModeMenu::OpenMenu, this, 2) , SCR_HEIGHT/4.f, 280.f, 200.f, SCR_HEIGHT*0.25f, mainContainer, "main_menu.options.text");
	Button * quitButton    = InitButton(std::bind(&ApplicationModeMenu::QuitGame, this)    , SCR_HEIGHT/4.f, 230.f, 200.f, SCR_HEIGHT*0.25f, mainContainer, "main_menu.exit.text", 1.f, 1.f, 0.f, 1.f, 0.7f, 0.f);

	mainContainer->FocusNextControl();
	mContainers.push_back(mainContainer);


	//Container select level menu
	NavigationContainer * levelContainer = new NavigationContainer();

	Button * level1Button       = InitButton(std::bind(&ApplicationModeMenu::StartLevel1 , this)    , SCR_HEIGHT/4.f, 330.f, 200.f, SCR_HEIGHT*0.25f, levelContainer, "main_menu.level1.text");
	Button * level2Button       = InitButton(std::bind(&ApplicationModeMenu::StartLevel2 , this)    , SCR_HEIGHT/4.f, 280.f, 200.f, SCR_HEIGHT*0.25f, levelContainer, "main_menu.level2.text");
	Button * level3Button       = InitButton(std::bind(&ApplicationModeMenu::StartLevel3 , this)    , SCR_HEIGHT/4.f, 230.f, 200.f, SCR_HEIGHT*0.25f, levelContainer, "main_menu.level3.text");
	Button * levelsReturnButton = InitButton(std::bind(&ApplicationModeMenu::OpenMenu    , this, 0) , SCR_HEIGHT/4.f, 180.f, 200.f, SCR_HEIGHT*0.25f, levelContainer, "main_menu.return.text", 1.f, 1.f, 0.f, 1.f, 0.7f, 0.f);

	levelContainer->SetVisible(false);
	mContainers.push_back(levelContainer);


	//Container options menu
	NavigationContainer * optionsContainer = new NavigationContainer();
		     
	Button   * spanishButton       = InitButton   (std::bind(&ApplicationModeMenu::ChangeLanguage , this, Properties::P_Language::Spanish) , SCR_HEIGHT/4.f, 330.f, 200.f, SCR_HEIGHT*0.25f, optionsContainer, "main_menu.spanish.text");
	Button   * englishButton       = InitButton   (std::bind(&ApplicationModeMenu::ChangeLanguage , this, Properties::P_Language::English) , SCR_HEIGHT/4.f, 280.f, 200.f, SCR_HEIGHT*0.25f, optionsContainer, "main_menu.english.text");
	Checkbox * audioCheckbox       = InitCheckbox (SCR_HEIGHT/4.f, 230.f, 250.f, SCR_HEIGHT*0.25f, optionsContainer, "main_menu.activateAudio.text", g_pApplicationManager->IsAudioActivated());
	mControlMap[audioCheckbox]     = std::bind(&ApplicationModeMenu::ChangeActivatedAudio, this, audioCheckbox);
	Button   * optionsReturnButton = InitButton   (std::bind(&ApplicationModeMenu::OpenMenu       , this, 0)                               , SCR_HEIGHT/4.f, 180.f, 200.f, SCR_HEIGHT*0.25f, optionsContainer, "main_menu.return.text", 1.f, 1.f, 0.f, 1.f, 0.7f, 0.f);

	optionsContainer->SetVisible(false);
	mContainers.push_back(optionsContainer);

	g_pGraphicsEngine->Init();
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::Deactivate()
{
	assert(g_pEventManager);

	Properties::RemoveInstance();
	m_pProperties = nullptr;

	auto it = mContainers.begin();
	while (mContainers.end() != it)
	{
		const std::vector<Control *> containerControls = (*it)->GetControls();
		for (auto control : containerControls) g_pEventManager->Unregister(control);

		(*it)->Clear();
		it = mContainers.erase(it);
	}

	g_pEventManager->Unregister(this);
	g_pGraphicsEngine->End();
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::ProcessInput()
{
	assert(g_pEventManager);
	g_pEventManager->UpdateEvents();
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::Run(float deltaTime)
{
	if (mCurrentContainer) mCurrentContainer->Update(deltaTime);
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::Render()
{
	glColor3f(0.f, 1.f, 1.f);
	glClear( GL_COLOR_BUFFER_BIT );

	FONT_DrawString(vmake(SCR_HEIGHT/4.f, 400.f), m_pProperties->GetProperty("main_menu.title.text").c_str());
	mCurrentContainer->Render();
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::ChangeLanguage(Properties::P_Language lang)
{
	Properties::P_Language applicationLanguage = g_pApplicationManager->GetLang();
	if (applicationLanguage != lang)
	{
		g_pApplicationManager->SetLang(lang);
		m_pProperties = Properties::Instance("messages", lang);
		assert(m_pProperties);

		for (auto container : mContainers)
		{
			const std::vector<Control *> containerControls = container->GetControls();
			for (auto control : containerControls)
			{
				Button * button = dynamic_cast<Button *>(control);
				if (button) button->SetProperties(m_pProperties);
				else
				{
					Checkbox  * checkbox = dynamic_cast<Checkbox *>(control);
					if (checkbox) checkbox->SetProperties(m_pProperties);
				}
			}
		}
	}
}

// *************************************************
//
// *************************************************

bool ApplicationModeMenu::ProcessEvent(IEventManager::EM_Event event)
{
	switch (event) 
	{
		case IEventManager::EM_Event::SinglePressUp     : mCurrentContainer->FocusPreviousControl(); break;
		case IEventManager::EM_Event::SinglePressDown   : mCurrentContainer->FocusNextControl();     break;
		case IEventManager::EM_Event::SinglePressEscape : QuitGame();                                break;
	}

	return true;
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::OnClick(Button * button)
{
	mControlMap[button]();
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::OnClick(Checkbox * checkbox)
{
	mControlMap[checkbox]();
}

// *************************************************
//
// *************************************************

Button * ApplicationModeMenu::InitButton(std::function<void()> clickFunction, float x, float y, float width, float height, Container * parent, const char * textKey, float rOn, float gOn, float bOn, float rOff, float gOff, float bOff)
{
	Button * button = new Button(x, y, width, height, parent, m_pProperties, textKey, rOn, gOn, bOn, rOff, gOff, bOff);

	button->SetListener(this);

	g_pEventManager->Register(button  , IEventManager::EM_Event::SinglePressEnter, 0);

	mControlMap[button] = clickFunction;

	return button;
}

// *************************************************
//
// *************************************************

Checkbox * ApplicationModeMenu::InitCheckbox(float x, float y, float width, float height, Container * parent, const char * textKey, bool checked, float rOn, float gOn, float bOn, float rOff, float gOff, float bOff)
{
	Checkbox * checkbox = new Checkbox(x, y, width, height, parent, m_pProperties, textKey, checked, rOn, gOn, bOn, rOff, gOff, bOff);

	checkbox->SetListener(this);

	g_pEventManager->Register(checkbox  , IEventManager::EM_Event::SinglePressEnter, 0);

	return checkbox;
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::StartLevel1()
{
	g_gameLevel = 1;
	g_pApplicationManager->SwitchMode(AM_Game);
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::StartLevel2()
{
	g_gameLevel = 2;
	g_pApplicationManager->SwitchMode(AM_Game);
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::StartLevel3()
{
	g_gameLevel = 3;
	g_pApplicationManager->SwitchMode(AM_Game);
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::OpenMenu(int index)
{
	if (mCurrentContainer)
	{
		mCurrentContainer->SetVisible(false);
		Control * focusedControl = mCurrentContainer->GetFocusedControl();
		if (focusedControl) focusedControl->SetFocused(false);
	}

	if (static_cast<int>(mContainers.size()) > index) mCurrentContainer = mContainers[index];
	//std::vector<Control *> containerControls = mCurrentContainer->GetControls();
	//if(!containerControls.empty()) containerControls[0]->SetFocused(true);
	
	if (mCurrentContainer)
	{
		mCurrentContainer->SetVisible(true);
		mCurrentContainer->ResetFocus();
	}
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::ChangeActivatedAudio(Checkbox * checkbox)
{
	bool checkboxChecked = !checkbox->IsChecked();
	checkbox->SetChecked(checkboxChecked);
	g_pApplicationManager->SetAudioActivated(checkboxChecked);
	// TODO poner audio en play o false
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::QuitGame()
{
	PostQuitMessage(0);
}