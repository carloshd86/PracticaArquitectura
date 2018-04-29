#ifndef __GLOBALS_H__
#define __GLOBALS_H__


#include "graphicsengine.h"
#include "soundmanager.h"
#include "eventmanager.h"
#include "properties.h"
#include "game.h"


class ApplicationManager;

extern ApplicationManager * g_pApplicationManager;
extern Game               * g_pGame;
extern Game::GameLevel      g_gameLevel;
extern IGraphicsEngine    * g_pGraphicsEngine;
extern ISoundManager      * g_pSoundManager;
extern IEventManager      * g_pEventManager;
											    
constexpr const char * LEVEL_JSON_KEY           = "level";
constexpr const char * GOAL_JSON_KEY            = "goal";
constexpr const char * PLAYER_JSON_KEY          = "player";
constexpr const char * ENEMIES_JSON_KEY         = "enemies";
constexpr const char * WALLS_JSON_KEY           = "walls";
constexpr const char * BACKGROUND_IMG_JSON_KEY  = "backgroundImg";
constexpr const char * BACKGROUND_R_JSON_KEY    = "backgroundR";
constexpr const char * BACKGROUND_G_JSON_KEY    = "backgroundG";
constexpr const char * BACKGROUND_B_JSON_KEY    = "backgroundB";
constexpr const char * SPEED_JSON_KEY           = "speed";
constexpr const char * PURSUING_SPEED_JSON_KEY  = "pursuingSpeed";
constexpr const char * ROUTE_POINTS_JSON_KEY    = "routePoints";
constexpr const char * POS_X_JSON_KEY           = "posX";
constexpr const char * POS_Y_JSON_KEY           = "posY";
constexpr const char * WIDTH_JSON_KEY           = "width";
constexpr const char * HEIGHT_JSON_KEY          = "height";
constexpr const char * IMG_JSON_KEY             = "img";
constexpr const char * IMG_LEFT_JSON_KEY        = "imgLeft";
constexpr const char * IMG_RIGHT_JSON_KEY       = "imgRight";
constexpr const char * IMG_ALERT_LEFT_JSON_KEY  = "imgAlertLeft";
constexpr const char * IMG_ALERT_RIGHT_JSON_KEY = "imgAlertRight";

const Properties::P_Language DEFAULT_LANG = Properties::P_Language::Spanish;

#endif
