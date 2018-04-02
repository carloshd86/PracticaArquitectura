#include "stdafx.h"
#include "globals.h"
#include "coresprite.h"

CoreSprite::CoreSprite(vec2 pos, vec2 size, uint32_t tex, float red, float green, float blue) :
	mPos(pos),
	mSize(size),
	mTex (tex),
	mColor { red, green, blue } {}

vec2 CoreSprite::GetPos() const
{
	return mPos;
}

void CoreSprite::SetPos(float x, float y)
{
	mPos.x = x;
	mPos.y = y;
}

vec2 CoreSprite::GetSize() const
{
	return mSize;
}

void CoreSprite::SetSize(float x, float y)
{
	mSize.x = x;
	mSize.y = y;
}

uint32_t CoreSprite::GetTex() const
{
	return mTex;
}

void CoreSprite::SetColor(float red, float green, float blue)
{
	mColor[0] = red;
	mColor[1] = green;
	mColor[2] = blue;
}

float CoreSprite::GetRed() const
{
	return mColor[0];
}

float CoreSprite::GetGreen() const
{
	return mColor[1];
}

float CoreSprite::GetBlue() const
{
	return mColor[2];
}