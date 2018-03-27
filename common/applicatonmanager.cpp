#include "stdafx.h"
#include "globals.h"
#include "applicationmanager.h"
#include "applicationmodegame.h"
#include "applicationmodemenu.h"


ApplicationManager::ApplicationManager() : m_desiredMode(AM_MENU), m_pGameMode(nullptr) {}

ApplicationManager::~ApplicationManager()
{
	delete m_pGameMode;
}

void ApplicationManager::SwitchMode(IdMode mode)
{
	m_desiredMode = mode;
}

void ApplicationManager::ManageModeChange()
{
	if (!m_pGameMode)
	{
		m_pGameMode = new ApplicationModeMenu();
		m_pGameMode->Activate();
	}
	else if (m_pGameMode->GetId() != m_desiredMode)
	{
		if (m_pGameMode) m_pGameMode->Deactivate();
		delete m_pGameMode;

		switch (m_desiredMode)
		{
			case AM_MENU: m_pGameMode = new ApplicationModeMenu();   break;
			case AM_GAME: m_pGameMode = new ApplicationModeGame();   break;
			default:      m_pGameMode = nullptr;                     break;
		}

		m_pGameMode->Activate();
	}
}

void ApplicationManager::ProcessInput()
{
	m_pGameMode->ProcessInput();
}

void ApplicationManager::Run(float deltaTime)
{
	m_pGameMode->Run(deltaTime);
}

void ApplicationManager::Render()
{
	m_pGameMode->Render();
}
