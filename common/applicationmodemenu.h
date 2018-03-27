#ifndef _APPLICATION_MODE_MENU_H_
#define _APPLICATION_MODE_MENU_H_


#include "applicationmode.h"


class ApplicationModeMenu : public ApplicationMode
{
public:

	ApplicationModeMenu();

	IdMode GetId           ();
	void   Activate        ();
	void   Deactivate      ();
	void   ProcessInput    ();
	void   Run             (float deltaTime);
	void   Render          ();

private:

	bool m_waitEscapeRelease;
};

#endif