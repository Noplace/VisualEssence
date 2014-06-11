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

#ifndef FONT_LOADER_H
#define FONT_LOADER_H


namespace acGraphics {

// Implement private helper classes for loading the bitmap font files

class FontLoader : public ve::Component {
 public:
	FontLoader(const wchar_t *fontFile);
	virtual int Load() = 0; // Must be implemented by derived class
  void LoadFont(Font* font) { font_ = font; Load();  }
 protected:
	ID3D11Resource* LoadPage(int id, const wchar_t *pageFile, const wchar_t *fontFile);
	void SetFontInfo(int outlineThickness);
	void SetCommonInfo(int fontHeight, int base, int scaleW, int scaleH, int pages, bool isPacked);
	void AddChar(int id, int x, int y, int w, int h, int xoffset, int yoffset, int xadvance, int page, int chnl);
	void AddKerningPair(int first, int second, int amount);

	FILE *f;
	Font *font_;
	const wchar_t *fontFile;

	int outlineThickness;
};

class FontLoaderBinaryFormat : public FontLoader {
public:
	FontLoaderBinaryFormat(const wchar_t *fontFile);

	int Load();

	void ReadInfoBlock(int size);
	void ReadCommonBlock(int size);
	void ReadPagesBlock(int size);
	void ReadCharsBlock(int size);
	void ReadKerningPairsBlock(int size);
};


}

#endif