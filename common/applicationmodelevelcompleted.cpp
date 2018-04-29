#include "stdafx.h"
#include "globals.h"
#include "applicationmodelevelcompleted.h"
#include "applicationmanager.h"
#include "sys.h"
#include "core.h"
#include "font.h"
#include <assert.h>


ApplicationModeLevelCompleted::ApplicationModeLevelCompleted () :
	mMusicId          (0) {}

// *************************************************
//
// *************************************************

ApplicationModeLevelCompleted::~ApplicationModeLevelCompleted()
{
}

// *************************************************
//
// *************************************************

IdMode ApplicationModeLevelCompleted::GetId()
{
	return AM_LevelCompleted;
}

// *************************************************
//
// *************************************************

void ApplicationModeLevelCompleted::Activate()
{
	assert(g_pEventManager);

	m_pProperties = Properties::Instance("messages", g_pApplicationManager->GetLang());
	assert(m_pProperties);

	g_pEventManager->Register(this, IEventManager::EM_Event::SinglePressEnter , 0);

	g_pGraphicsEngine->Init();

	mMusicId = g_pSoundManager->LoadWav("../data/DefenseLine.wav");
	if (mMusicId && g_pApplicationManager->IsAudioActivated()) g_pSoundManager->PlayMusic(mMusicId);
}

// *************************************************
//
// *************************************************

void ApplicationModeLevelCompleted::Deactivate()
{
	assert(g_pEventManager);

	Properties::RemoveInstance();
	m_pProperties = nullptr;

	g_pEventManager->Unregister(this);
	g_pSoundManager->UnloadWav(mMusicId);
	g_pGraphicsEngine->End();
}

// *************************************************
//
// *************************************************

void ApplicationModeLevelCompleted::ProcessInput()
{
	assert(g_pEventManager);
	g_pEventManager->UpdateEvents();
}

// *************************************************
//
// *************************************************

void ApplicationModeLevelCompleted::Run(float deltaTime)
{
}

// *************************************************
//
// *************************************************

void ApplicationModeLevelCompleted::Render()
{
	glClear( GL_COLOR_BUFFER_BIT );

	glColor3f(1.f, 1.f, 0.f);
	if (g_gameLevel == Game::FINAL_GAME_LEVEL) FONT_DrawString(vmake(SCR_HEIGHT/6.f, 350.f), m_pProperties->GetProperty("level_completed_final.title.text").c_str());
	else                                       FONT_DrawString(vmake(SCR_HEIGHT/6.f, 350.f), m_pProperties->GetProperty("level_completed.title.text").c_str());

	glColor3f(1.f, 1.f, 1.f);
	FONT_DrawString(vmake(SCR_HEIGHT/6.f, 300.f), m_pProperties->GetProperty("game.press_enter.text").c_str());
	if (g_gameLevel == Game::FINAL_GAME_LEVEL) FONT_DrawString(vmake(20.f, 270.f), m_pProperties->GetProperty("level_completed_final.press_enter.text").c_str());
	else                                       FONT_DrawString(vmake(20.f, 270.f), m_pProperties->GetProperty("level_completed.press_enter.text").c_str());
}

// *************************************************
//
// *************************************************

void ApplicationModeLevelCompleted::ChangeLanguage(Properties::P_Language lang)
{
	Properties::P_Language applicationLanguage = g_pApplicationManager->GetLang();
	if (applicationLanguage != lang)
	{
		g_pApplicationManager->SetLang(lang);
		m_pProperties = Properties::Instance("messages", lang);
		assert(m_pProperties);
	}
}

// *************************************************
//
// *************************************************

bool ApplicationModeLevelCompleted::ProcessEvent(IEventManager::EM_Event event)
{
	switch (event) 
	{
		case IEventManager::EM_Event::SinglePressEnter: 
			if (g_gameLevel == Game::FINAL_GAME_LEVEL) g_pApplicationManager->SwitchMode(AM_Menu); 
			else                                     { g_pApplicationManager->SwitchMode(AM_Game); g_gameLevel = Game::GetNextLevel(g_gameLevel); }
			break;
	}

	return true;
}
