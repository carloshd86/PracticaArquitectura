#ifndef __APPLICATION_MANAGER_H__
#define __APPLICATION_MANAGER_H__


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

	IdMode           mDesiredMode;
	ApplicationMode *m_pGameMode;
};

#endif
