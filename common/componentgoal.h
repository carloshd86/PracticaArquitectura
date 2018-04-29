#ifndef __COMPONENT_GOAL_H__
#define __COMPONENT_GOAL_H__


#include "component.h"


class C_Goal : public Component
{
public:

	C_Goal(Entity * owner) :
		Component(owner) {}

	virtual ~C_Goal()
	{
		End();
	}

	virtual void Init()
	{
		mInitialized = true;
	}

	virtual void End()
	{
		mInitialized = false;
	}

	virtual void Run(float deltaTime)
	{
	}

private:

	bool mInitialized;
};

#endif
