#ifndef _APPLICATION_MANAGER_H_
#define _APPLICATION_MANAGER_H_


#include "applicationmode.h"


class ApplicationManager
{
public:

	ApplicationManager();
	~ApplicationManager();

	void SwitchMode        (IdMode mode);
	void ManageModeChange  ();
	void ProcessInput      ();
	void Run               (float deltaTime);
	void Render            ();
	
private:

	IdMode           m_desiredMode;
	ApplicationMode *m_pGameMode;
};

#endif
