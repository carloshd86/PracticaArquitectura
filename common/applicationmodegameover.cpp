#include "stdafx.h"
#include "globals.h"
#include "applicationmodegameover.h"
#include "applicationmanager.h"
#include "sys.h"
#include "core.h"
#include "font.h"
#include <assert.h>


ApplicationModeGameOver::ApplicationModeGameOver () :
	mMusicId          (0) {}

// *************************************************
//
// *************************************************

ApplicationModeGameOver::~ApplicationModeGameOver()
{
}

// *************************************************
//
// *************************************************

IdMode ApplicationModeGameOver::GetId()
{
	return AM_GameOver;
}

// *************************************************
//
// *************************************************

void ApplicationModeGameOver::Activate()
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

void ApplicationModeGameOver::Deactivate()
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

void ApplicationModeGameOver::ProcessInput()
{
	assert(g_pEventManager);
	g_pEventManager->UpdateEvents();
}

// *************************************************
//
// *************************************************

void ApplicationModeGameOver::Run(float deltaTime)
{
}

// *************************************************
//
// *************************************************

void ApplicationModeGameOver::Render()
{
	glColor3f(1.f, 1.f, 1.f);
	glClear( GL_COLOR_BUFFER_BIT );

	glColor3f(1.f, 0.f, 0.f);
	FONT_DrawString(vmake(SCR_HEIGHT/6.f, 350.f), m_pProperties->GetProperty("game_over.title.text").c_str());

	glColor3f(1.f, 1.f, 1.f);
	FONT_DrawString(vmake(20.f, 300.f), m_pProperties->GetProperty("game_over.press_enter.text").c_str());
}

// *************************************************
//
// *************************************************

void ApplicationModeGameOver::ChangeLanguage(Properties::P_Language lang)
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

bool ApplicationModeGameOver::ProcessEvent(IEventManager::EM_Event event)
{
	switch (event) 
	{
		case IEventManager::EM_Event::SinglePressEnter: g_pApplicationManager->SwitchMode(AM_Menu); break;
	}

	return true;
}
