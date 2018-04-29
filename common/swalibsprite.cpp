#include "stdafx.h"
#include "globals.h"
#include "swalibsprite.h"


SwalibSprite::SwalibSprite(vec2 pos, vec2 size, uint32_t tex, float red, float green, float blue) :
	mPos(pos),
	mSize(size),
	mTex (tex),
	mColor { red, green, blue } {}

// *************************************************
//
// *************************************************

vec2 SwalibSprite::GetPos() const
{
	return mPos;
}

// *************************************************
//
// *************************************************

void SwalibSprite::SetPos(float x, float y)
{
	mPos.x = x;
	mPos.y = y;
}

// *************************************************
//
// *************************************************

vec2 SwalibSprite::GetSize() const
{
	return mSize;
}

// *************************************************
//
// *************************************************

void SwalibSprite::SetSize(float x, float y)
{
	mSize.x = x;
	mSize.y = y;
}

// *************************************************
//
// *************************************************

uint32_t SwalibSprite::GetTex() const
{
	return mTex;
}

// *************************************************
//
// *************************************************

void SwalibSprite::SetColor(float red, float green, float blue)
{
	mColor[0] = red;
	mColor[1] = green;
	mColor[2] = blue;
}

// *************************************************
//
// *************************************************

float SwalibSprite::GetRed() const
{
	return mColor[0];
}

// *************************************************
//
// *************************************************

float SwalibSprite::GetGreen() const
{
	return mColor[1];
}

// *************************************************
//
// *************************************************

float SwalibSprite::GetBlue() const
{
	return mColor[2];
}