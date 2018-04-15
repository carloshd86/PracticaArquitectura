#ifndef __NAVICATION_CONTAINER_H__
#define __NAVICATION_CONTAINER_H__


#include "container.h"


class Control;

class NavigationContainer : public Container
{

public:

	NavigationContainer(bool allowNoFocused = false);
	virtual ~NavigationContainer();

	virtual void Update (float deltaTime);

	virtual Control * GetFocusedControl    ();
	virtual void      FocusNextControl     ();
	virtual void      FocusPreviousControl ();
	virtual void      ResetFocus           ();

private:

	int  mFocusedIndex;
	bool mAllowNoFocused;
};

#endif