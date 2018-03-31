#include "stdafx.h"
#include "globals.h"
#include "sys.h"
#include "core.h"
#include "font.h"
#include "applicationmanager.h"
#include "game.h"
#include "coregraphicsengine.h"
#include "sysinputmanager.h"


//-----------------------------------------------------------------------------

ApplicationManager * g_pApplicationManager;
Game               * g_pGame;
int                  g_gameLevel;
IGraphicsEngine    * g_pGraphicsEngine;
IEventManager      * g_pEventManager;

//-----------------------------------------------------------------------------
int Main(void)
{
  g_pApplicationManager = new ApplicationManager();
 
  g_pGraphicsEngine     = new CoreGraphicsEngine();
  g_pGraphicsEngine->Init();

  g_pEventManager = SysInputManager::Instance();

  FONT_Init();

  // Set up rendering
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // Sets up clipping
  glClearColor( 0.0f, 0.1f, 0.3f, 0.0f );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( 0.0, SCR_WIDTH, 0.0, SCR_HEIGHT, 0.0, 1.0);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  float deltaTime = 1 / 60.f;

  while (!SYS_GottaQuit())
  {
	g_pApplicationManager->ManageModeChange();

	//ProcessInput
	g_pApplicationManager->ProcessInput();


	//Run
	g_pApplicationManager->Run(deltaTime);


    // Render
	g_pApplicationManager->Render();

    
    SYS_Show();
	

    // Keep system running
    SYS_Pump();
    SYS_Sleep(17);
  }

  FONT_End();

  delete g_pGame;
  delete g_pGraphicsEngine;
  delete g_pEventManager;
  delete g_pApplicationManager;
  
  return 0;
}
