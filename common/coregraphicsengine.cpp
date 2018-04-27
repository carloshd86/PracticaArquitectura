#include "stdafx.h"
#include "globals.h"
#include "coregraphicsengine.h"
#include "sys.h"
#include "core.h"
#include "coresprite.h"


CoreGraphicsEngine::CoreGraphicsEngine(const char * backgroundImage) : 
	mBackgroundImage (backgroundImage),
	mBackgroundR     (1.f),
	mBackgroundG     (1.f),
	mBackgroundB     (1.f),
	mOverlayActive   (false),
	mOverlayR        (0.f),
	mOverlayG        (0.f),
	mOverlayB        (0.f),
	mOverlayA        (0.6f),
	mInitialized     (false), 
	mEnded           (false) {}

// *************************************************
//
// *************************************************

CoreGraphicsEngine::CoreGraphicsEngine() : 
	CoreGraphicsEngine ("") {}

// *************************************************
//
// *************************************************

CoreGraphicsEngine::~CoreGraphicsEngine()
{
	if (mInitialized && !mEnded) End();
}

// *************************************************
//
// *************************************************

void CoreGraphicsEngine::Init()
{
	if (!mInitialized)
	{
		if (!mBackgroundImage.empty()) mBackground = CORE_LoadPNG(mBackgroundImage.c_str(), true);
		mOverlayActive = false;

		mInitialized = true;
	}
}

// *************************************************
//
// *************************************************

void CoreGraphicsEngine::End()
{
	if (mInitialized)
	{
		for(auto sprite : mSprites) delete sprite;
		mSprites.clear();

		for (auto& texture : mTextures) CORE_UnloadPNG(texture.second);
		mTextures.clear();

		mInitialized = false;
	}
	mEnded = true;
}

// *************************************************
//
// *************************************************

void CoreGraphicsEngine::Render()
{
	glColor3f(mBackgroundR, mBackgroundG, mBackgroundB);
	glClear( GL_COLOR_BUFFER_BIT );

	// Render background
	if (!mBackgroundImage.empty())
	{
		for (int i = 0; i <= SCR_WIDTH / GAME_BACKGROUND_WIDTH; i++)
			for (int j = 0; j <= SCR_HEIGHT / GAME_BACKGROUND_HEIGHT; j++)
				CORE_RenderCenteredSprite(vmake(i * GAME_BACKGROUND_WIDTH + 64.f, j * GAME_BACKGROUND_HEIGHT + 64.f), vmake(GAME_BACKGROUND_WIDTH, GAME_BACKGROUND_HEIGHT), mBackground);
	}

	// Render elements
	for(auto sprite : mSprites)
	{
		glColor3f(sprite->GetRed(), sprite->GetGreen(), sprite->GetBlue());
		vec2 pos = sprite->GetPos();
		vec2 size = sprite->GetSize();
		CORE_RenderSprite(pos, vmake(pos.x + size.x, pos.y + size.y), sprite->GetTex());
	}

	// Render overlay
	if (mOverlayActive)
	{
		glColor4f(mOverlayR, mOverlayG, mOverlayB, mOverlayA);
		CORE_RenderSprite(vmake(0.f, 0.f), vmake(SCR_WIDTH, SCR_HEIGHT), 0);
	}
}

// *************************************************
//
// *************************************************

ISprite * CoreGraphicsEngine::RequireSprite(vec2 pos, vec2 size, const char * image, float r, float g, float b)
{
	GLuint sprite_id = 0;
	for (auto& texture : mTextures) 
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
	
	ISprite * requir_sprite = new CoreSprite(pos, size, sprite_id, r, g, b);

	mSprites.push_back(requir_sprite);

	return requir_sprite;
}

// *************************************************
//
// *************************************************

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

// *************************************************
//
// *************************************************

void CoreGraphicsEngine::SetOverlayActive(bool active)
{
	mOverlayActive = active;
}

// *************************************************
//
// *************************************************

void CoreGraphicsEngine::SetOverlayColor(float r, float g, float b, float a)
{
	mOverlayR = r;
	mOverlayG = g;
	mOverlayB = b;
}

// *************************************************
//
// *************************************************

void CoreGraphicsEngine::SetBackgroundImage(const char * backgroundImage)
{
	if(!mBackgroundImage.empty()) CORE_UnloadPNG(mBackground);
	mBackgroundImage = backgroundImage;
	mBackground = CORE_LoadPNG(mBackgroundImage.c_str(), true);
}

// *************************************************
//
// *************************************************

void CoreGraphicsEngine::SetBackgroundColor(float r, float g, float b)
{
	mBackgroundR = r;
	mBackgroundG = g;
	mBackgroundB = b;
}