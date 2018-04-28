#include "stdafx.h"
#include "globals.h"
#include "swalibgraphicsengine.h"
#include "sys.h"
#include "core.h"
#include "swalibsprite.h"


SwalibGraphicsEngine::SwalibGraphicsEngine(const char * backgroundImage) : 
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

SwalibGraphicsEngine::SwalibGraphicsEngine() : 
	SwalibGraphicsEngine ("") {}

// *************************************************
//
// *************************************************

SwalibGraphicsEngine::~SwalibGraphicsEngine()
{
	if (mInitialized && !mEnded) End();
}

// *************************************************
//
// *************************************************

void SwalibGraphicsEngine::Init()
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

void SwalibGraphicsEngine::End()
{
	if (mInitialized)
	{
		for(auto sprite : mSprites) delete sprite.first;
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

void SwalibGraphicsEngine::Render()
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
	for(auto spritePair : mSprites)
	{
		if (spritePair.second)
		{
			ISprite * sprite = spritePair.first;
			glColor3f(sprite->GetRed(), sprite->GetGreen(), sprite->GetBlue());
			vec2 pos = sprite->GetPos();
			vec2 size = sprite->GetSize();
			CORE_RenderSprite(pos, vmake(pos.x + size.x, pos.y + size.y), sprite->GetTex());
		}
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

ISprite * SwalibGraphicsEngine::RequireSprite(vec2 pos, vec2 size, const char * image, bool manageRender, float r, float g, float b)
{
	GLuint spriteId = 0;
	for (auto& texture : mTextures) 
	{
		if (texture.first == image) 
		{
			spriteId = texture.second;
			break;
		}
	}

	if (!spriteId) 
	{
		spriteId = CORE_LoadPNG(image, false);
		mTextures.push_back({ image, spriteId });
	}
	
	ISprite * requiredSprite = new SwalibSprite(pos, size, spriteId, r, g, b);
	mSprites.push_back(std::pair<ISprite *, bool>(requiredSprite, manageRender));

	return requiredSprite;
}

// *************************************************
//
// *************************************************

void SwalibGraphicsEngine::ReleaseSprite(ISprite * sprite) 
{
	for (auto it = mSprites.begin(); it != mSprites.end(); ) 
	{
		if ((*it).first == sprite) 
		{
			delete (*it).first;  
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

void SwalibGraphicsEngine::SetOverlayActive(bool active)
{
	mOverlayActive = active;
}

// *************************************************
//
// *************************************************

void SwalibGraphicsEngine::SetOverlayColor(float r, float g, float b, float a)
{
	mOverlayR = r;
	mOverlayG = g;
	mOverlayB = b;
}

// *************************************************
//
// *************************************************

void SwalibGraphicsEngine::SetBackgroundImage(const char * backgroundImage)
{
	if(!mBackgroundImage.empty()) CORE_UnloadPNG(mBackground);
	mBackgroundImage = backgroundImage;
	mBackground = CORE_LoadPNG(mBackgroundImage.c_str(), true);
}

// *************************************************
//
// *************************************************

void SwalibGraphicsEngine::SetBackgroundColor(float r, float g, float b)
{
	mBackgroundR = r;
	mBackgroundG = g;
	mBackgroundB = b;
}