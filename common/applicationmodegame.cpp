#include "stdafx.h"
#include "globals.h"
#include "applicationmodegame.h"
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
}

void ApplicationModeGame::Deactivate()
{
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
	g_pGame->GetGraphicsEngine()->Render();
}