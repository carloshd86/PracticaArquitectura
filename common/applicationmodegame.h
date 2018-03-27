#ifndef _APPLICATION_MODE_GAME_H_
#define _APPLICATION_MODE_GAME_H_


#include "applicationmode.h"


class ApplicationModeGame : public ApplicationMode
{
public:

	IdMode GetId           ();
	void   Activate        ();
	void   Deactivate      ();
	void   ProcessInput    ();
	void   Run             (float deltaTime);
	void   Render          ();
};

#endif