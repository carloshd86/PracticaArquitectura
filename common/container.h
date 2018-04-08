#ifndef __CONTAINER_H__
#define __CONTAINER_H__


#include "eventmanager.h"
#include <vector>


class Control;

class Container 
{

public:

	Container();
	virtual ~Container();

	virtual const std::vector<Control *>& GetControls () const;
	virtual void                          AddControl    (Control * control);
	virtual void                          RemoveControl (Control *& control);
	virtual void                          Clear         ();

	virtual void Update (float deltaTime);
	virtual void Render ();

	virtual bool GetVisible () const;
	virtual void SetVisible (bool visible);

protected:

	std::vector<Control *> mControls;
	bool                   mVisible;
};

#endif