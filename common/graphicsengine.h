#ifndef __GRAPHICS_ENGINE_H__
#define __GRAPHICS_ENGINE_H__


#include "sprite.h"


class IGraphicsEngine
{

public:

	virtual void      Init               ()                                                                                                               = 0;
	virtual void      End                ()                                                                                                               = 0;
	virtual void      Render             ()                                                                                                               = 0;
	virtual ISprite * RequireSprite      (vec2 pos, vec2 size, const char * image, bool manageRender = true, float r = 1.f, float g = 1.f, float b = 1.f) = 0;
	virtual void      ReleaseSprite      (ISprite * sprite)                                                                                               = 0;
	virtual void      RenderSprite       (ISprite * sprite)                                                                                               = 0;
	virtual void      SetOverlayActive   (bool active)                                                                                                    = 0;
	virtual void      SetOverlayColor    (float r, float g, float b, float a)                                                                             = 0;
	virtual void      SetBackgroundImage (const char * backgroundImage)                                                                                   = 0;
	virtual void      SetBackgroundColor (float r, float g, float b)                                                                                      = 0;
	virtual vec2      GetWorldSize       ()                                                                                                               = 0;
};

#endif