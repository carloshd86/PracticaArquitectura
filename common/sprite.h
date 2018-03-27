#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "core.h"
#include <array>


class Sprite
{
public:

	Sprite(vec2 pos, vec2 size, GLuint tex, float red, float green, float blue);

	vec2   GetPos               ()                                    const; 
	void   SetPos               (float x, float y); 
	vec2   GetSize              ()                                    const; 
	void   SetSize              (float x, float y); 
	GLuint GetTex               ()                                    const; 
	void   SetColor             (float red, float green, float blue);
	float  GetRed               ()                                    const;
	float  GetGreen             ()                                    const;
	float  GetBlue              ()                                    const;

private:

	vec2                   m_pos;
	vec2                   m_size;
	GLuint                 m_tex;
	std::array<float, 3>   m_color;
};

#endif