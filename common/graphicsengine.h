#ifndef __GRAPHICS_ENGINE_H__
#define __GRAPHICS_ENGINE_H__


#include "sprite.h"


class IGraphicsEngine
{
public:

	virtual void      Init          ()                                                                                              = 0;
	virtual void      End           ()                                                                                              = 0;
	virtual void      Render        ()                                                                                              = 0;
	virtual ISprite * RequireSprite (vec2 pos, vec2 size, const char * image, float red = 1.f, float green = 1.f, float blue = 1.f) = 0;
	virtual void      ReleaseSprite (ISprite * sprite)                                                                              = 0;
};

#endif