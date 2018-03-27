#include "stdafx.h"
#include "globals.h"
#include "gameinputmanager.h"
#include "applicationmanager.h"
#include "game.h"
#include "sys.h"
#include "entity.h"
#include "messages.h"
#include <assert.h>

GameInputManager::GameInputManager() {}

void GameInputManager::ProcessInput()
{
	assert(g_pGame);

	Entity * player = g_pGame->GetPlayer();

	assert(player);

	player->ReceiveMessage(SetMovementMessage(0.f, 0.f));

	if (SYS_KeyPressed(SYS_KEY_UP))      player->ReceiveMessage(MoveUpMessage());
	if (SYS_KeyPressed(SYS_KEY_DOWN))    player->ReceiveMessage(MoveDownMessage());
	if (SYS_KeyPressed(SYS_KEY_LEFT))    player->ReceiveMessage(MoveLeftMessage());
	if (SYS_KeyPressed(SYS_KEY_RIGHT))   player->ReceiveMessage(MoveRightMessage());
	if (SYS_KeyPressed(VK_ESCAPE))       g_pApplicationManager->SwitchMode(AM_MENU);
}