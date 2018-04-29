#ifndef __APPLICATION_MODE_LEVEL_COMPLETED_H__
#define __APPLICATION_MODE_LEVEL_COMPLETED_H__


#include "applicationmode.h"


class ApplicationModeLevelCompleted : public ApplicationMode, public IEventManager::IListener
{
public:

	ApplicationModeLevelCompleted();
	~ApplicationModeLevelCompleted();

	IdMode GetId           ();
	void   Activate        ();
	void   Deactivate      ();
	void   ProcessInput    ();
	void   Run             (float deltaTime);
	void   Render          ();
	void   ChangeLanguage  (Properties::P_Language lang);

	bool ProcessEvent (IEventManager::EM_Event event);


private:

	Properties  *m_pProperties;
	unsigned int mMusicId;
	bool         mExitScreen;
};

#endif