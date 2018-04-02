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
	mWaitEscapeRelease(false) {}

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

	g_pEventManager->Register(this, IEventManager::EM_Event::SinglePressUp     , 0);
	g_pEventManager->Register(this, IEventManager::EM_Event::SinglePressDown   , 0);
	g_pEventManager->Register(this, IEventManager::EM_Event::SinglePressEscape , 0);

	Button * level1Button = new Button(SCR_HEIGHT/4.f, SCR_HEIGHT*0.75f, 200.f, SCR_HEIGHT*0.25f, &mContainer, "START LEVEL 1");
	level1Button->SetFocused(true);
	Button * level2Button = new Button(SCR_HEIGHT/4.f, SCR_HEIGHT*0.5f , 200.f, SCR_HEIGHT*0.25f, &mContainer, "START LEVEL 2");
	Button * level3Button = new Button(SCR_HEIGHT/4.f, SCR_HEIGHT*0.25f, 200.f, SCR_HEIGHT*0.25f, &mContainer, "START LEVEL 3");
	
	level1Button->SetListener(this);
	level2Button->SetListener(this);
	level3Button->SetListener(this);

	g_pEventManager->Register(level1Button, IEventManager::EM_Event::SinglePressEnter, 0);
	g_pEventManager->Register(level2Button, IEventManager::EM_Event::SinglePressEnter, 0);
	g_pEventManager->Register(level3Button, IEventManager::EM_Event::SinglePressEnter, 0);

	mButtonMap[level1Button] = std::bind(&ApplicationModeMenu::StartLevel1, this);
	mButtonMap[level2Button] = std::bind(&ApplicationModeMenu::StartLevel2, this);
	mButtonMap[level3Button] = std::bind(&ApplicationModeMenu::StartLevel3, this);
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::Deactivate()
{
	assert(g_pEventManager);

	g_pEventManager->Unregister(this);
	const std::vector<Control *> containerControls = mContainer.GetControls();
	for(auto control : containerControls) g_pEventManager->Unregister(control);

	g_pEventManager->Unregister(this);

	mContainer.Clear();
}

// *************************************************
//
// *************************************************

void ApplicationModeMenu::ProcessInput()
{
	assert(g_pEventManager);
	g_pEventManager->UpdateEvents();
	/*if      (SYS_KeyPressed(KEY_LEVEL_1)) { g_gameLevel = 1; g_pApplicationManager->SwitchMode(AM_GAME); }
	else if (SYS_KeyPressed(KEY_LEVEL_2)) { g_gameLevel = 2; g_pApplicationManager->SwitchMode(AM_GAME); }
	else if (SYS_KeyPressed(KEY_LEVEL_3)) { g_gameLevel = 3; g_pApplicationManager->SwitchMode(AM_GAME); }
*/
	/*if (mWaitEscapeRelease)
	{
		if (!SYS_KeyPressed(VK_ESCAPE)) mWaitEscapeRelease = false;
	}
	else if (SYS_KeyPressed(VK_ESCAPE)) PostQuitMessage(0);*/
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
	/*FONT_DrawString(vmake(SCR_HEIGHT/4.f, SCR_HEIGHT*0.75f), "PRESS 1 TO START LEVEL 1");
	FONT_DrawString(vmake(SCR_HEIGHT/4.f, SCR_HEIGHT*0.5f) , "PRESS 2 TO START LEVEL 2");
	FONT_DrawString(vmake(SCR_HEIGHT/4.f, SCR_HEIGHT*0.25f), "PRESS 3 TO START LEVEL 3");*/
	mContainer.Render();
}

// *************************************************
//
// *************************************************

bool ApplicationModeMenu::ProcessEvent(IEventManager::EM_Event event)
{
	switch (event) 
	{
		case IEventManager::EM_Event::SinglePressUp      : mContainer.FocusPreviousControl(); break;
		case IEventManager::EM_Event::SinglePressDown    : mContainer.FocusNextControl();     break;
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