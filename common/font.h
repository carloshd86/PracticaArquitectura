#ifndef __FONT_H__
#define __FONT_H__

void FONT_Init();
void FONT_End();
void FONT_DrawChar(vec2 p0, vec2 p1, unsigned char ch);
void FONT_DrawString(vec2 p0, const char string[]);
vec2 FONT_GetStringNextPos(vec2 p0, const char string[]);

#endif
