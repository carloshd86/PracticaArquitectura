#include "stdafx.h"
#include "globals.h"
#include "applicationmodegame.h"
#include "applicationmanager.h"
#include "game.h"
#include "gameinputmanager.h"
#include "graphicsengine.h"
#include "sys.h"
#include "core.h"
#include "font.h"
#include <assert.h>


ApplicationModeGame::ApplicationModeGame () :
	mCurrentContainer (nullptr),
	mInGameMenuActive (false) {}

// *************************************************
//
// *************************************************

ApplicationModeGame::~ApplicationModeGame()
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

IdMode ApplicationModeGame::GetId()
{
	return AM_GAME;
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::Activate()
{
	assert(g_pEventManager);
	assert(g_gameLevel>= 1 && g_gameLevel <= 3);

	// Properties
	m_pProperties = Properties::Instance("messages", g_pApplicationManager->GetLang());
	assert(m_pProperties);

	// Register events for this 
	g_pEventManager->Register(this, IEventManager::EM_Event::SinglePressUp     , 0);
	g_pEventManager->Register(this, IEventManager::EM_Event::SinglePressDown   , 0);
	g_pEventManager->Register(this, IEventManager::EM_Event::SinglePressEscape , 0);

	// Container main menu
	NavigationContainer * mainContainer = new NavigationContainer();

	Button * levelsButton     = new Button(SCR_HEIGHT/4.f, 380.f, 200.f, SCR_HEIGHT*0.25f, mainContainer, m_pProperties, "main_menu.levels.text");
	Button * optionsButton    = new Button(SCR_HEIGHT/4.f, 330.f, 200.f, SCR_HEIGHT*0.25f, mainContainer, m_pProperties, "main_menu.options.text");

	levelsButton->SetListener(this);
	optionsButton->SetListener(this);

	g_pEventManager->Register(levelsButton     , IEventManager::EM_Event::SinglePressEnter, 0);
	g_pEventManager->Register(optionsButton    , IEventManager::EM_Event::SinglePressEnter, 0);

	mButtonMap[levelsButton]  = std::bind(&ApplicationModeGame::OpenMenu, this, 1);
	mButtonMap[optionsButton] = std::bind(&ApplicationModeGame::OpenMenu, this, 2);

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

	mButtonMap[level1Button]       = std::bind(&ApplicationModeGame::QuitGame , this);
	mButtonMap[level2Button]       = std::bind(&ApplicationModeGame::QuitGame , this);
	mButtonMap[level3Button]       = std::bind(&ApplicationModeGame::QuitGame , this);
	mButtonMap[levelsReturnButton] = std::bind(&ApplicationModeGame::OpenMenu , this, 0);

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

	mButtonMap[spanishButton]       = std::bind(&ApplicationModeGame::ChangeLanguage , this, Properties::P_Language::Spanish);
	mButtonMap[englishButton]       = std::bind(&ApplicationModeGame::ChangeLanguage , this, Properties::P_Language::English);
	mButtonMap[optionsReturnButton] = std::bind(&ApplicationModeGame::OpenMenu       , this, 0);

	optionsContainer->SetVisible(false);
	mContainers.push_back(optionsContainer);

	// Game
	if (!g_pGame) g_pGame = new Game();
	switch (g_gameLevel)
	{
		case 1: g_pGame->SetGameLevel(Game::GameLevel::LEVEL_1); break;
		case 2: g_pGame->SetGameLevel(Game::GameLevel::LEVEL_2); break;
		case 3: g_pGame->SetGameLevel(Game::GameLevel::LEVEL_3); break;
	}
	g_pGame->Init();
	g_pGraphicsEngine->Init();
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::Deactivate()
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
	g_pGame->End();
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::ProcessInput()
{
	assert(g_pEventManager);
	g_pEventManager->UpdateEvents();
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::Run(float deltaTime)
{
	if (!mInGameMenuActive) g_pGame->Run(deltaTime);
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::Render()
{
	g_pGraphicsEngine->Render();
	if (mInGameMenuActive) mCurrentContainer->Render();
}

// *************************************************
//
// *************************************************

bool ApplicationModeGame::ProcessEvent(IEventManager::EM_Event event) 
{

	switch (event) {
	    case IEventManager::EM_Event::SinglePressUp    : if (mInGameMenuActive) mCurrentContainer->FocusPreviousControl(); break;
	    case IEventManager::EM_Event::SinglePressDown  : if (mInGameMenuActive) mCurrentContainer->FocusNextControl();     break;
		case IEventManager::EM_Event::SinglePressEscape:
			if (mInGameMenuActive) 
				ResumeGame();
			else 
				OpenMenu(0);           
			break;
	}

	return true;
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::OnClick(Button * button)
{
	mButtonMap[button]();
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::ChangeLanguage(Properties::P_Language lang)
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
			}
		}
	}
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::ResumeGame() 
{
	mInGameMenuActive = false;
	if (mCurrentContainer)
	{
		mCurrentContainer->SetVisible(false);
		Control * focusedControl = mCurrentContainer->GetFocusedControl();
		if (focusedControl) focusedControl->SetFocused(false);
		mCurrentContainer = nullptr;
	}
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::QuitGame() 
{
	g_pApplicationManager->SwitchMode(AM_MENU);
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::OpenMenu(int index)
{
	if (!mInGameMenuActive)
	{
		mInGameMenuActive = true;
	}

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
		mCurrentContainer->FocusNextControl();
	}
}