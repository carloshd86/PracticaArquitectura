#include "stdafx.h"
#include "globals.h"
#include "graphicsengine.h"
#include "sys.h"
#include "core.h"
#include "sprite.h"


GraphicsEngine::GraphicsEngine() : m_initialized(false), m_ended(false) {}

GraphicsEngine::~GraphicsEngine()
{
	if (m_initialized && !m_ended) End();
}

void GraphicsEngine::Init()
{
	if (!m_initialized)
	{
		m_background = CORE_LoadPNG(GAME_BACKGROUND, true);
		m_initialized = true;
	}
}

void GraphicsEngine::End()
{
	if (m_initialized)
	{
		for(auto sprite : m_sprites) delete sprite;
		m_sprites.clear();

		for (auto texture : m_textures) CORE_UnloadPNG(texture.second);
		m_textures.clear();

		m_initialized = false;
	}
	m_ended = true;
}


void GraphicsEngine::Render()
{
	glClear( GL_COLOR_BUFFER_BIT );

	// Render background
	for (int i = 0; i <= SCR_WIDTH/GAME_BACKGROUND_WIDTH; i++)
		for (int j = 0; j <= SCR_HEIGHT/GAME_BACKGROUND_HEIGHT; j++)
			CORE_RenderCenteredSprite(vmake(i * GAME_BACKGROUND_WIDTH + 64.f, j * GAME_BACKGROUND_HEIGHT + 64.f), vmake(GAME_BACKGROUND_WIDTH, GAME_BACKGROUND_HEIGHT), m_background);

	// Render elements
	for(auto sprite : m_sprites)
	{
		glColor3f(sprite->GetRed(), sprite->GetGreen(), sprite->GetBlue());
		vec2 pos = sprite->GetPos();
		vec2 size = sprite->GetSize();
		CORE_RenderSprite(pos, vmake(pos.x + size.x, pos.y + size.y), sprite->GetTex());
	}
}


Sprite * GraphicsEngine::RequireSprite(vec2 pos, vec2 size, const char * image, float red, float green, float blue)
{
	GLuint sprite_id = 0;
	for (auto texture : m_textures) 
	{
		if (texture.first == image) 
		{
			sprite_id = texture.second;
			break;
		}
	}

	if (!sprite_id) 
	{
		sprite_id = CORE_LoadPNG(image, false);
		m_textures.push_back({ image, sprite_id });
	}
	
	Sprite * required_sprite = new Sprite(pos, size, sprite_id, red, green, blue);

	m_sprites.push_back(required_sprite);

	return required_sprite;
}

void GraphicsEngine::ReleaseSprite(Sprite * sprite) 
{
	for (auto it = m_sprites.begin(); it != m_sprites.end(); ) 
	{
		if ((*it) == sprite) 
		{
			delete * it;  
			it = m_sprites.erase(it);
		} 
		else 
		{
			++it;
		}
	}
}