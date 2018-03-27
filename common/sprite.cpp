#include "stdafx.h"
#include "globals.h"
#include "sprite.h"

Sprite::Sprite(vec2 pos, vec2 size, GLuint tex, float red, float green, float blue) :
	m_pos(pos),
	m_size(size),
	m_tex (tex),
	m_color { red, green, blue } {}

vec2 Sprite::GetPos() const
{
	return m_pos;
}

void Sprite::SetPos(float x, float y)
{
	m_pos.x = x;
	m_pos.y = y;
}

vec2 Sprite::GetSize() const
{
	return m_size;
}

void Sprite::SetSize(float x, float y)
{
	m_size.x = x;
	m_size.y = y;
}

GLuint Sprite::GetTex() const
{
	return m_tex;
}

void Sprite::SetColor(float red, float green, float blue)
{
	m_color[0] = red;
	m_color[1] = green;
	m_color[2] = blue;
}

float Sprite::GetRed() const
{
	return m_color[0];
}

float Sprite::GetGreen() const
{
	return m_color[1];
}

float Sprite::GetBlue() const
{
	return m_color[2];
}