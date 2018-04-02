#include "stdafx.h"
#include "globals.h"
#include "coregraphicsengine.h"
#include "sys.h"
#include "core.h"
#include "coresprite.h"


CoreGraphicsEngine::CoreGraphicsEngine() : mInitialized(false), mEnded(false) {}

CoreGraphicsEngine::~CoreGraphicsEngine()
{
	if (mInitialized && !mEnded) End();
}

void CoreGraphicsEngine::Init()
{
	if (!mInitialized)
	{
		mBackground = CORE_LoadPNG(GAME_BACKGROUND, true);
		mInitialized = true;
	}
}

void CoreGraphicsEngine::End()
{
	if (mInitialized)
	{
		for(auto sprite : mSprites) delete sprite;
		mSprites.clear();

		for (auto texture : mTextures) CORE_UnloadPNG(texture.second);
		mTextures.clear();

		mInitialized = false;
	}
	mEnded = true;
}


void CoreGraphicsEngine::Render()
{
	glClear( GL_COLOR_BUFFER_BIT );

	// Render background
	for (int i = 0; i <= SCR_WIDTH/GAME_BACKGROUND_WIDTH; i++)
		for (int j = 0; j <= SCR_HEIGHT/GAME_BACKGROUND_HEIGHT; j++)
			CORE_RenderCenteredSprite(vmake(i * GAME_BACKGROUND_WIDTH + 64.f, j * GAME_BACKGROUND_HEIGHT + 64.f), vmake(GAME_BACKGROUND_WIDTH, GAME_BACKGROUND_HEIGHT), mBackground);

	// Render elements
	for(auto sprite : mSprites)
	{
		glColor3f(sprite->GetRed(), sprite->GetGreen(), sprite->GetBlue());
		vec2 pos = sprite->GetPos();
		vec2 size = sprite->GetSize();
		CORE_RenderSprite(pos, vmake(pos.x + size.x, pos.y + size.y), sprite->GetTex());
	}
}


ISprite * CoreGraphicsEngine::RequireSprite(vec2 pos, vec2 size, const char * image, float red, float green, float blue)
{
	GLuint sprite_id = 0;
	for (auto texture : mTextures) 
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
		mTextures.push_back({ image, sprite_id });
	}
	
	ISprite * required_sprite = new CoreSprite(pos, size, sprite_id, red, green, blue);

	mSprites.push_back(required_sprite);

	return required_sprite;
}

void CoreGraphicsEngine::ReleaseSprite(ISprite * sprite) 
{
	for (auto it = mSprites.begin(); it != mSprites.end(); ) 
	{
		if ((*it) == sprite) 
		{
			delete * it;  
			it = mSprites.erase(it);
		} 
		else 
		{
			++it;
		}
	}
}