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
	~CoreGraphicsEngine();

	void      Init          ();
	void      End           ();
	void      Render        ();
	ISprite * RequireSprite (vec2 pos, vec2 size, const char * image, float red = 1.f, float green = 1.f, float blue = 1.f);
	void      ReleaseSprite (ISprite * sprite);

private:

	const char * GAME_BACKGROUND        = "../data/256xdrygrassblock.png";
	const float	 GAME_BACKGROUND_WIDTH  = 128;
	const float	 GAME_BACKGROUND_HEIGHT = 128;

	bool                                          m_initialized;
	bool                                          m_ended;
	uint32_t                                      m_background;
	std::vector<ISprite *>                        m_sprites;
	std::vector<std::pair<std::string, uint32_t>> m_textures;
};

#endif