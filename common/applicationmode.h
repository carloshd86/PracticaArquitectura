#ifndef __APPLICATION_MODE_H__
#define __APPLICATION_MODE_H__


#include "properties.h"


enum IdMode 
{
	AM_NULL,
	AM_MENU,
	AM_GAME
};

class ApplicationMode 
{
public:

	virtual IdMode GetId           ()  =0;
	virtual void   Activate        ()  =0;
	virtual void   Deactivate      ()  =0;
	virtual void   ProcessInput    ()  =0;
	virtual void   Run             (float deltaTime)  =0;
	virtual void   Render          ()  =0;
	virtual void   ChangeLanguage  (Properties::P_Language lang)  =0;
};

#endif
