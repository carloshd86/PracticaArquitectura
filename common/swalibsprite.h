#ifndef __SWALIB_SPRITE_H__
#define __SWALIB_SPRITE_H__

#include "sprite.h"
#include <array>
#include <stdint.h>


class SwalibSprite : public ISprite
{
public:

	SwalibSprite(vec2 pos, vec2 size, uint32_t tex, float red, float green, float blue);

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

	vec2                   mPos;
	vec2                   mSize;
	uint32_t               mTex;
	std::array<float, 3>   mColor;
};

#endif