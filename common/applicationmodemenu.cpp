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
	return AM_MENU;
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

	Button * levelsButton     = new Button(SCR_HEIGHT/4.f, 380.f, 200.f, SCR_HEIGHT*0.25f, mainContainer, m_pProperties, "main_menu.levels.text");
	Button * optionsButton    = new Button(SCR_HEIGHT/4.f, 330.f, 200.f, SCR_HEIGHT*0.25f, mainContainer, m_pProperties, "main_menu.options.text");

	levelsButton->SetListener(this);
	optionsButton->SetListener(this);

	g_pEventManager->Register(levelsButton     , IEventManager::EM_Event::SinglePressEnter, 0);
	g_pEventManager->Register(optionsButton    , IEventManager::EM_Event::SinglePressEnter, 0);

	mButtonMap[levelsButton]  = std::bind(&ApplicationModeMenu::OpenMenu    , this, 1);
	mButtonMap[optionsButton] = std::bind(&ApplicationModeMenu::OpenMenu    , this, 2);

	mainContainer->FocusNextControl();
	mContainers.push_back(mainContainer);


	//Container select level menu
	NavigationContainer * levelContainer = new NavigationContainer();

	Button * level1Button       = new Button(SCR_HEIGHT/4.f, 380.f, 200.f, SCR_HEIGHT*0.25f, levelContainer, m_pProperties, "main_menu.level1.text");
	Button * level2Button       = new Button(SCR_HEIGHT/4.f, 330.f, 200.f, SCR_HEIGHT*0.25f, levelContainer, m_pProperties, "main_menu.level2.text");
	Button * level3Button       = new Button(SCR_HEIGHT/4.f, 280.f, 200.f, SCR_HEIGHT*0.25f, levelContainer, m_pProperties, "main_menu.level3.text");
	Button * levelsReturnButton = new Button(SCR_HEIGHT/4.f, 230.f, 200.f, SCR_HEIGHT*0.25f, levelContainer, m_pProperties, "main_menu.return.text", 1.f, 1.f, 0.f, 1.f, 0.7f, 0.f);

	level1Button->SetListener(this);
	level2Button->SetListener(this);
	level3Button->SetListener(this);
	levelsReturnButton->SetListener(this);

	g_pEventManager->Register(level1Button       , IEventManager::EM_Event::SinglePressEnter, 0);
	g_pEventManager->Register(level2Button       , IEventManager::EM_Event::SinglePressEnter, 0);
	g_pEventManager->Register(level3Button       , IEventManager::EM_Event::SinglePressEnter, 0);
	g_pEventManager->Register(levelsReturnButton , IEventManager::EM_Event::SinglePressEnter, 0);

	mButtonMap[level1Button]       = std::bind(&ApplicationModeMenu::StartLevel1 , this);
	mButtonMap[level2Button]       = std::bind(&ApplicationModeMenu::StartLevel2 , this);
	mButtonMap[level3Button]       = std::bind(&ApplicationModeMenu::StartLevel3 , this);
	mButtonMap[levelsReturnButton] = std::bind(&ApplicationModeMenu::OpenMenu    , this, 0);

	levelContainer->SetVisible(false);
	mContainers.push_back(levelContainer);


	//Container options menu
	NavigationContainer * optionsContainer = new NavigationContainer();

	Button * spanishButton       = new Button(SCR_HEIGHT/4.f, 380.f, 200.f, SCR_HEIGHT*0.25f, optionsContainer, m_pProperties, "main_menu.spanish.text");
	Button * englishButton       = new Button(SCR_HEIGHT/4.f, 330.f, 200.f, SCR_HEIGHT*0.25f, optionsContainer, m_pProperties, "main_menu.english.text");
	Button * optionsReturnButton = new Button(SCR_HEIGHT/4.f, 280.f, 200.f, SCR_HEIGHT*0.25f, optionsContainer, m_pProperties, "main_menu.return.text", 1.f, 1.f, 0.f, 1.f, 0.7f, 0.f);

	spanishButton->SetListener(this);
	englishButton->SetListener(this);
	optionsReturnButton->SetListener(this);

	g_pEventManager->Register(spanishButton       , IEventManager::EM_Event::SinglePressEnter, 0);
	g_pEventManager->Register(englishButton       , IEventManager::EM_Event::SinglePressEnter, 0);
	g_pEventManager->Register(optionsReturnButton , IEventManager::EM_Event::SinglePressEnter, 0);

	mButtonMap[spanishButton]       = std::bind(&ApplicationModeMenu::ChangeLanguage , this, Properties::P_Language::Spanish);
	mButtonMap[englishButton]       = std::bind(&ApplicationModeMenu::ChangeLanguage , this, Properties::P_Language::English);
	mButtonMap[optionsReturnButton] = std::bind(&ApplicationModeMenu::OpenMenu       , this, 0);

	optionsContainer->SetVisible(false);
	mContainers.push_back(optionsContainer);
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::Deactivate()
{
	assert(g_pEventManager);

	Properties::RemoveInstance();
	m_pProperties = nullptr;

	g_pEventManager->Unregister(this);

	auto it = mContainers.begin();
	while (mContainers.end() != it)
	{
		const std::vector<Control *> containerControls = (*it)->GetControls();
		for (auto control : containerControls) g_pEventManager->Unregister(control);

		(*it)->Clear();
		it = mContainers.erase(it);
	}

	g_pEventManager->Unregister(this);
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
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::Render()
{
	glClear( GL_COLOR_BUFFER_BIT );
	mCurrentContainer->Render();
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::ChangeLanguage(Properties::P_Language lang)
{
	// TODO
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
		case IEventManager::EM_Event::SinglePressEscape : PostQuitMessage(0);                break;
	}

	return true;
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::OnClick(Button * button)
{
	mButtonMap[button]();
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::StartLevel1()
{
	g_gameLevel = 1;
	g_pApplicationManager->SwitchMode(AM_GAME);
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::StartLevel2()
{
	g_gameLevel = 2;
	g_pApplicationManager->SwitchMode(AM_GAME);
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::StartLevel3()
{
	g_gameLevel = 3;
	g_pApplicationManager->SwitchMode(AM_GAME);
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