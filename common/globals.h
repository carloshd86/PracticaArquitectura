#ifndef __GLOBALS_H__
#define __GLOBALS_H__


#include "graphicsengine.h"
#include "eventmanager.h"
#include "properties.h"


class ApplicationManager;
class Game;

extern ApplicationManager * g_pApplicationManager;
extern Game               * g_pGame;
extern int                  g_gameLevel;
extern IGraphicsEngine    * g_pGraphicsEngine;
extern IEventManager      * g_pEventManager;


const int KEY_LEVEL_1 = 0x31;
const int KEY_LEVEL_2 = 0x32;
const int KEY_LEVEL_3 = 0x33;

constexpr const char * GOAL_JSON_KEY           = "goal";
constexpr const char * PLAYER_JSON_KEY         = "player";
constexpr const char * ENEMIES_JSON_KEY        = "enemies";
constexpr const char * WALLS_JSON_KEY          = "walls";
constexpr const char * SPEED_JSON_KEY          = "speed";
constexpr const char * PURSUING_SPEED_JSON_KEY = "pursuingSpeed";
constexpr const char * ROUTE_POINTS_JSON_KEY   = "routePoints";
constexpr const char * POS_X_JSON_KEY          = "posX";
constexpr const char * POS_Y_JSON_KEY          = "posY";
constexpr const char * WIDTH_JSON_KEY          = "width";
constexpr const char * HEIGHT_JSON_KEY         = "height";
constexpr const char * IMG_JSON_KEY            = "img";

const Properties::P_Language DEFAULT_LANG = Properties::P_Language::Spanish;

#endif
