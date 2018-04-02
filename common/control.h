#ifndef __CONTROL_H__
#define __CONTROL_H__


#include "eventmanager.h"
#include "container.h"


class Control : public IEventManager::IListener 
{

public:

	Control(float x, float y, float width, float height, Container * parent);
	virtual ~Control();

	virtual bool ProcessEvent (IEventManager::EM_Event event) = 0;
	virtual void Update       (float deltaTime)               = 0;
	virtual void Render       ()                              = 0;

	virtual const Container * GetParent  () const;
	virtual void              SetFocused (bool focused);


protected:

	float      mX;
	float      mY;
	float      mWidth;
	float      mHeight;
	bool       mFocused;
	Container *mParent;
};


#endif