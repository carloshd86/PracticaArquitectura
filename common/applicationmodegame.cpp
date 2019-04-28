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
#include "asserts.h"
#include "memorycontrol.h"


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
		GAME_DELETE((*it));
		it = mContainers.erase(it);
	}
}

// *************************************************
//
// *************************************************

IdMode ApplicationModeGame::GetId()
{
	return AM_Game;
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::Activate()
{
	GAME_ASSERT(g_pEventManager);

	// Properties
	m_pProperties = Properties::Instance("messages", g_pApplicationManager->GetLang());
	GAME_ASSERT(m_pProperties);

	// Register events for this 
	g_pEventManager->Register(this, IEventManager::EM_Event::SinglePressUp     , 0);
	g_pEventManager->Register(this, IEventManager::EM_Event::SinglePressDown   , 0);
	g_pEventManager->Register(this, IEventManager::EM_Event::SinglePressEscape , 0);

	// Container main menu
	NavigationContainer * mainContainer = GAME_NEW(NavigationContainer, ());
	mCurrentContainer = mainContainer;

	Button * resumeGameButton = InitButton(std::bind(&ApplicationModeGame::ResumeGame, this)  , SCR_HEIGHT/4.f, 330.f, 200.f, SCR_HEIGHT*0.25f, mainContainer, "main_menu.resume_game.text");
	Button * optionsButton    = InitButton(std::bind(&ApplicationModeGame::OpenMenu, this, 1) , SCR_HEIGHT/4.f, 280.f, 200.f, SCR_HEIGHT*0.25f, mainContainer, "main_menu.options.text");
	Button * quitButton       = InitButton(std::bind(&ApplicationModeGame::QuitGame, this)    , SCR_HEIGHT/4.f, 230.f, 200.f, SCR_HEIGHT*0.25f, mainContainer, "main_menu.exit.text", 1.f, 1.f, 0.f, 1.f, 0.7f, 0.f);

	mainContainer->SetVisible(false);
	mContainers.push_back(mainContainer);


	//Container options menu
	NavigationContainer * optionsContainer = GAME_NEW(NavigationContainer, ());

	Button   * spanishButton       = InitButton   (std::bind(&ApplicationModeGame::ChangeLanguage , this, Properties::P_Language::Spanish) , SCR_HEIGHT/4.f, 380.f, 200.f, SCR_HEIGHT*0.25f, optionsContainer, "main_menu.spanish.text");
	Button   * englishButton       = InitButton   (std::bind(&ApplicationModeGame::ChangeLanguage , this, Properties::P_Language::English) , SCR_HEIGHT/4.f, 330.f, 200.f, SCR_HEIGHT*0.25f, optionsContainer, "main_menu.english.text");
	Checkbox * audioCheckbox       = InitCheckbox (                                                                                          SCR_HEIGHT/4.f, 280.f, 250.f, SCR_HEIGHT*0.25f, optionsContainer, "main_menu.activate_audio.text", g_pApplicationManager->IsAudioActivated());
	mControlMap[audioCheckbox]     = std::bind(&ApplicationModeGame::ChangeActivatedAudio, this, audioCheckbox);
	Button   * optionsReturnButton = InitButton   (std::bind(&ApplicationModeGame::OpenMenu       , this, 0)                               , SCR_HEIGHT/4.f, 230.f, 200.f, SCR_HEIGHT*0.25f, optionsContainer, "main_menu.return.text", 1.f, 1.f, 0.f, 1.f, 0.7f, 0.f);

	optionsContainer->SetVisible(false);
	mContainers.push_back(optionsContainer);

	// Game
	if (!g_pGame) g_pGame = GAME_NEW(Game, ());
	g_pGame->SetGameLevel(g_gameLevel);

	g_pGame->Init();
	g_pGraphicsEngine->Init();
	g_pGraphicsEngine->SetOverlayActive(false);

	mMusicId = g_pSoundManager->LoadWav((DATA_FOLDER + "ArcadeFunk.wav").c_str());
	ChangeActivatedAudio(audioCheckbox);
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::Deactivate()
{
	GAME_ASSERT(g_pEventManager);

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
	g_pSoundManager->UnloadWav(mMusicId);
	g_pGraphicsEngine->End();
	g_pGame->End();
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::ProcessInput()
{
	GAME_ASSERT(g_pEventManager);
	g_pEventManager->UpdateEvents();
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::Run(float deltaTime)
{
	if (!mInGameMenuActive) g_pGame->Run(deltaTime);
	else if (mCurrentContainer) mCurrentContainer->Update(deltaTime);
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
	mControlMap[button]();
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::OnClick(Checkbox * checkbox)
{
	mControlMap[checkbox]();
}

// *************************************************
//
// *************************************************

Button * ApplicationModeGame::InitButton(std::function<void()> clickFunction, float x, float y, float width, float height, Container * parent, const char * textKey, float rOn, float gOn, float bOn, float rOff, float gOff, float bOff)
{
	Button * button = GAME_NEW(Button, (x, y, width, height, parent, m_pProperties, textKey, rOn, gOn, bOn, rOff, gOff, bOff));

	button->SetListener(this);

	g_pEventManager->Register(button  , IEventManager::EM_Event::SinglePressEnter, 0);

	mControlMap[button] = clickFunction;

	return button;
}

// *************************************************
//
// *************************************************

Checkbox * ApplicationModeGame::InitCheckbox(float x, float y, float width, float height, Container * parent, const char * textKey, bool checked, float rOn, float gOn, float bOn, float rOff, float gOff, float bOff)
{
	Checkbox * checkbox = GAME_NEW(Checkbox, (x, y, width, height, parent, m_pProperties, textKey, checked, rOn, gOn, bOn, rOff, gOff, bOff));

	checkbox->SetListener(this);

	g_pEventManager->Register(checkbox, IEventManager::EM_Event::SinglePressEnter, 0);

	return checkbox;
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
		GAME_ASSERT(m_pProperties);

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

void ApplicationModeGame::ResumeGame() 
{
	g_pGraphicsEngine->SetOverlayActive(false);
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
	g_pApplicationManager->SwitchMode(AM_Menu);
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::OpenMenu(int index)
{
	if (!mInGameMenuActive)
	{
		mInGameMenuActive = true;
		g_pGraphicsEngine->SetOverlayActive(true);
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
		mCurrentContainer->ResetFocus();
	}
}

// *************************************************
//
// *************************************************

void ApplicationModeGame::ChangeActivatedAudio(Checkbox * checkbox)
{
	bool checkboxChecked = checkbox->IsChecked();
	g_pApplicationManager->SetAudioActivated(checkboxChecked);
	if (mMusicId) checkboxChecked ? g_pSoundManager->PlayMusic(mMusicId, g_pApplicationManager->GetVolume() / 100.f) : g_pSoundManager->StopMusic();
}