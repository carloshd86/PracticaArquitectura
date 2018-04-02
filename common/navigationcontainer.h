#ifndef __NAVICATION_CONTAINER_H__
#define __NAVICATION_CONTAINER_H__


#include "container.h"


class Control;

class NavigationContainer : public Container
{

public:

	NavigationContainer();
	virtual ~NavigationContainer();

	virtual Control * GetFocusedControl    ();
	virtual void      FocusNextControl     ();
	virtual void      FocusPreviousControl ();

private:

	int mFocusedIndex;
};

#endif