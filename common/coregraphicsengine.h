#ifndef __CORE_GRAPHICS_ENGINE_H__
#define __CORE_GRAPHICS_ENGINE_H__

#include "core.h"
#include "graphicsengine.h"
#include <vector>


class CoreSprite;

class CoreGraphicsEngine : public IGraphicsEngine
{
public:

	CoreGraphicsEngine();
	CoreGraphicsEngine(const char * backgroundImage);
	~CoreGraphicsEngine();

	void      Init               ();
	void      End                ();
	void      Render             ();
	ISprite * RequireSprite      (vec2 pos, vec2 size, const char * image, float r = 1.f, float g = 1.f, float b = 1.f);
	void      ReleaseSprite      (ISprite * sprite);
	void      SetOverlayActive   (bool active);
	void      SetOverlayColor    (float r, float g, float b, float a);
	void      SetBackgroundImage (const char * backgroundImage);
	void      SetBackgroundColor (float r, float g, float b);

private:

	const float	 GAME_BACKGROUND_WIDTH  = 128;
	const float	 GAME_BACKGROUND_HEIGHT = 128;

	bool                                          mInitialized;
	bool                                          mEnded;
	std::string                                   mBackgroundImage;
	uint32_t                                      mBackground;
	float                                         mBackgroundR;
	float                                         mBackgroundG;
	float                                         mBackgroundB;
	bool                                          mOverlayActive;
	float                                         mOverlayR;
	float                                         mOverlayG;
	float                                         mOverlayB;
	float                                         mOverlayA;
	std::vector<ISprite *>                        mSprites;
	std::vector<std::pair<std::string, uint32_t>> mTextures;
};

#endif