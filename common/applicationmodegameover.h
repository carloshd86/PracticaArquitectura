#ifndef __APPLICATION_MODE_GAME_OVER_H__
#define __APPLICATION_MODE_GAME_OVER_H__


#include "applicationmode.h"


class ApplicationModeGameOver : public ApplicationMode, public IEventManager::IListener
{
public:

	ApplicationModeGameOver();
	~ApplicationModeGameOver();

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
	ISprite     *m_pSprite;
};

#endif