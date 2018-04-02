#ifndef __CONTAINER_H__
#define __CONTAINER_H__


#include "eventmanager.h"
#include <vector>


class Control;

class Container 
{

public:

	virtual ~Container();

	virtual const std::vector<Control *>& GetControls () const;
	virtual void                        AddControl    (Control * control);
	virtual void                        RemoveControl (Control *& control);
	virtual void                        Clear         ();

	virtual void Update (float deltaTime);
	virtual void Render ();

protected:

	std::vector<Control *> mControls;
};

#endif