#ifndef _GRAPHICS_ENGINE_H_
#define _GRAPHICS_ENGINE_H_

#include "core.h"
#include "entity.h"
#include <vector>


class Sprite;

class GraphicsEngine
{
public:

	GraphicsEngine();
	~GraphicsEngine();

	void      Init          ();
	void      End           ();
	void      Render        ();
	Sprite *  RequireSprite (vec2 pos, vec2 size, const char * image, float red = 1.f, float green = 1.f, float blue = 1.f);
	void      ReleaseSprite (Sprite * sprite);

private:

	const char * GAME_BACKGROUND        = "../data/256xdrygrassblock.png";
	const float	 GAME_BACKGROUND_WIDTH  = 128;
	const float	 GAME_BACKGROUND_HEIGHT = 128;

	bool                                          m_initialized;
	bool                                          m_ended;
	GLuint                                        m_background;
	std::vector<Sprite *>                         m_sprites;
	std::vector<std::pair<std::string, GLuint>>   m_textures;
};

#endif