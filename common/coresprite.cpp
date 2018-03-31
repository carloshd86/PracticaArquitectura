#include "stdafx.h"
#include "globals.h"
#include "coresprite.h"

CoreSprite::CoreSprite(vec2 pos, vec2 size, uint32_t tex, float red, float green, float blue) :
	m_pos(pos),
	m_size(size),
	m_tex (tex),
	m_color { red, green, blue } {}

vec2 CoreSprite::GetPos() const
{
	return m_pos;
}

void CoreSprite::SetPos(float x, float y)
{
	m_pos.x = x;
	m_pos.y = y;
}

vec2 CoreSprite::GetSize() const
{
	return m_size;
}

void CoreSprite::SetSize(float x, float y)
{
	m_size.x = x;
	m_size.y = y;
}

uint32_t CoreSprite::GetTex() const
{
	return m_tex;
}

void CoreSprite::SetColor(float red, float green, float blue)
{
	m_color[0] = red;
	m_color[1] = green;
	m_color[2] = blue;
}

float CoreSprite::GetRed() const
{
	return m_color[0];
}

float CoreSprite::GetGreen() const
{
	return m_color[1];
}

float CoreSprite::GetBlue() const
{
	return m_color[2];
}