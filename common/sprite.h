#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "core.h"

class ISprite
{

public:

	virtual vec2         GetPos   ()                                    const = 0; 
	virtual void         SetPos   (float x, float y)                          = 0; 
	virtual vec2         GetSize  ()                                    const = 0; 
	virtual void         SetSize  (float x, float y)                          = 0; 
	virtual unsigned int GetTex   ()                                    const = 0; 
	virtual void         SetColor (float red, float green, float blue)        = 0;
	virtual float        GetRed   ()                                    const = 0;
	virtual float        GetGreen ()                                    const = 0;
	virtual float        GetBlue  ()                                    const = 0;
};

#endif