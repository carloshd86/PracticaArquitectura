#ifndef __CORE_SPRITE_H__
#define __CORE_SPRITE_H__

#include "sprite.h"
#include <array>
#include <stdint.h>


class CoreSprite : public ISprite
{
public:

	CoreSprite(vec2 pos, vec2 size, uint32_t tex, float red, float green, float blue);

	vec2         GetPos       ()                                    const; 
	void         SetPos       (float x, float y); 
	vec2         GetSize      ()                                    const; 
	void         SetSize      (float x, float y); 
	uint32_t     GetTex       ()                                    const; 
	void         SetColor     (float red, float green, float blue);
	float        GetRed       ()                                    const;
	float        GetGreen     ()                                    const;
	float        GetBlue      ()                                    const;

private:

	vec2                   m_pos;
	vec2                   m_size;
	uint32_t           m_tex;
	std::array<float, 3>   m_color;
};

#endif