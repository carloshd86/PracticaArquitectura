#include "stdafx.h"
#include "globals.h"
#include "applicationmodemenu.h"
#include "applicationmanager.h"
#include "sys.h"
#include "core.h"
#include "font.h"


ApplicationModeMenu::ApplicationModeMenu () :
	m_waitEscapeRelease(false) {}

IdMode ApplicationModeMenu::GetId()
{
	return AM_MENU;
}

void ApplicationModeMenu::Activate()
{
	if (SYS_KeyPressed(VK_ESCAPE))
		m_waitEscapeRelease = true;
}

void ApplicationModeMenu::Deactivate()
{
}

void ApplicationModeMenu::ProcessInput()
{
	if (SYS_KeyPressed(KEY_LEVEL_1))      { g_gameLevel = 1; g_pApplicationManager->SwitchMode(AM_GAME); }
	else if (SYS_KeyPressed(KEY_LEVEL_2)) { g_gameLevel = 2; g_pApplicationManager->SwitchMode(AM_GAME); }
	else if (SYS_KeyPressed(KEY_LEVEL_3)) { g_gameLevel = 3; g_pApplicationManager->SwitchMode(AM_GAME); }

	if (m_waitEscapeRelease)
	{
		if (!SYS_KeyPressed(VK_ESCAPE)) m_waitEscapeRelease = false;
	}
	else if (SYS_KeyPressed(VK_ESCAPE)) PostQuitMessage(0);
}

void ApplicationModeMenu::Run(float deltaTime)
{
}

void ApplicationModeMenu::Render()
{
	glClear( GL_COLOR_BUFFER_BIT );
	FONT_DrawString(vmake(SCR_HEIGHT/4.f, SCR_HEIGHT*0.75f), "PRESS 1 TO START LEVEL 1");
	FONT_DrawString(vmake(SCR_HEIGHT/4.f, SCR_HEIGHT*0.5f) , "PRESS 2 TO START LEVEL 2");
	FONT_DrawString(vmake(SCR_HEIGHT/4.f, SCR_HEIGHT*0.25f), "PRESS 3 TO START LEVEL 3");
}