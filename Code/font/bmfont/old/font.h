/*
   AngelCode Tool Box Library
   Copyright (c) 2007-2008 Andreas Jonsson
  
   This software is provided 'as-is', without any express or implied 
   warranty. In no event will the authors be held liable for any 
   damages arising from the use of this software.

   Permission is granted to anyone to use this software for any 
   purpose, including commercial applications, and to alter it and 
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you 
      must not claim that you wrote the original software. If you use
      this software in a product, an acknowledgment in the product 
      documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and 
      must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source 
      distribution.
  
   Andreas Jonsson
   andreas@angelcode.com
*/

#ifndef GRAPHICS_FONT_BMFONT_FONT_H
#define GRAPHICS_FONT_BMFONT_FONT_H

#include <stdio.h>
#include <vector>
#include <map>
#include "../../context.h"
#include "../../component.h"


namespace graphics {
namespace font {
namespace bmfont {

struct CharacterDescription
{
	CharacterDescription() : srcX(0), srcY(0), srcW(0), srcH(0), xOff(0), yOff(0), xAdv(0), page(0) {}

	short srcX;
	short srcY;
	short srcW;
	short srcH;
	short xOff;
	short yOff;
	short xAdv;
	short page;
	unsigned int chnl;

	std::vector<int> kerningPairs;
};

enum EFontTextEncoding
{
	NONE,
	UTF8,
	UTF16
};

const int FONT_ALIGN_LEFT    = 0;
const int FONT_ALIGN_CENTER  = 1;
const int FONT_ALIGN_RIGHT   = 2;
const int FONT_ALIGN_JUSTIFY = 3;

class Font : public Component
{
public:
	Font();
	~Font();

	int Initialize(const char *fontFile, const char *fxFile, Context* context);

	void SetTextEncoding(EFontTextEncoding encoding);

	float GetTextWidth(const char *text, int count);
	void Write(float x, float y, float z, const char *text, int count, unsigned int mode);
	void WriteML(float x, float y, float z, const char *text, int count, unsigned int mode);
	void WriteBox(float x, float y, float z, float width, const char *text, int count, unsigned mode);

	void SetHeight(float h);
	float GetHeight();

	float GetBottomOffset();
	float GetTopOffset();

	void PrepareEffect();
	void PreparePixelPerfectOutput();
  
protected:
	friend class Loader;

	void InternalWrite(float x, float y, float z, const char *text, int count, float spacing = 0);

	float AdjustForKerningPairs(int first, int second);
	CharacterDescription *GetChar(int id);

	int GetTextLength(const char *text);
	int GetTextChar(const char *text, int pos, int *nextPos = 0);
	int FindTextChar(const char *text, int start, int length, int ch);

	short fontHeight; // total height of the font
	short base;       // y of base line
	short scaleW;
	short scaleH;
	CharacterDescription defChar;
	bool hasOutline;

	float scale;
	EFontTextEncoding encoding;

	std::map<int, CharacterDescription*> chars;
	std::vector<IDirect3DTexture9*> pages;
	std::string fxFile;

  VertexBuffer<GameVertex> vb;
};

}
}
}

#endif