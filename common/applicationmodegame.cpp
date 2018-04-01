#include "stdafx.h"
#include "globals.h"
#include "applicationmodegame.h"
#include "applicationmanager.h"
#include "game.h"
#include "gameinputmanager.h"
#include "graphicsengine.h"
#include <assert.h>


IdMode ApplicationModeGame::GetId()
{
	return AM_GAME;
}

void ApplicationModeGame::Activate()
{
	assert(g_gameLevel>= 1 && g_gameLevel <= 3);

	if (!g_pGame) g_pGame = new Game();
	switch (g_gameLevel)
	{
		case 1: g_pGame->SetGameLevel(Game::GameLevel::LEVEL_1); break;
		case 2: g_pGame->SetGameLevel(Game::GameLevel::LEVEL_2); break;
		case 3: g_pGame->SetGameLevel(Game::GameLevel::LEVEL_3); break;
	}
	g_pGame->Init();
	g_pGraphicsEngine->Init();

	g_pEventManager->Register(this, IEventManager::EM_Event::PressEscape, 0);
}

void ApplicationModeGame::Deactivate()
{
	g_pEventManager->Unregister(this);
	g_pGraphicsEngine->End();
	g_pGame->End();
}

void ApplicationModeGame::ProcessInput()
{
	g_pGame->GetInputManager()->ProcessInput();
}

void ApplicationModeGame::Run(float deltaTime)
{
	g_pGame->Run(deltaTime);
}

void ApplicationModeGame::Render()
{
	g_pGraphicsEngine->Render();
}

bool ApplicationModeGame::ProcessEvent(IEventManager::EM_Event event) {

	switch (event) {
		case IEventManager::EM_Event::PressEscape: g_pApplicationManager->SwitchMode(AM_MENU); break;
	}

	return true;
}