#ifndef __CONTAINER_H__
#define __CONTAINER_H__


#include "eventmanager.h"
#include <vector>


class Control;

class Container 
{

public:

	~Container();

	const std::vector<Control *>& GetControls () const;
	void                        AddControl    (Control * control);
	void                        RemoveControl (Control *& control);
	void                        Clear         ();

	void Update (float deltaTime);
	void Render ();

private:

	std::vector<Control *> mControls;
};

#endif