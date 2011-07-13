#include "font_loader.h"

using namespace std;

namespace acGraphics {

FontLoader::FontLoader(FILE *f, const char *fontFile) {
	this->f = f;
	this->fontFile = fontFile;

	outlineThickness = 0;
}

void FontLoader::LoadPage(int id, const char *pageFile, const char *fontFile) {
	string str;

	// Load the texture from the same directory as the font descriptor file

	// Find the directory
	str = fontFile;
	for( size_t n = 0; (n = str.find('/', n)) != string::npos; ) str.replace(n, 1, "\\");
	size_t i = str.rfind('\\');
	if( i != string::npos )
		str = str.substr(0, i+1);
	else
		str = "";

	// Load the font textures
	str += pageFile;
	/*IDirect3DTexture9 *texture = 0;
	UINT mipLevels = 0; // 0 = all
	HRESULT hr = D3DXCreateTextureFromFileEx(font->render->GetDevice(), str.c_str(), 
		D3DX_DEFAULT, D3DX_DEFAULT, mipLevels, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
		D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &texture);
	if( FAILED(hr) )
	{
		LOG(("Failed to load font page '%s' (%X)", str.c_str(), hr));
	}*/

	font_->pages[id] = 0;
}

void FontLoader::SetFontInfo(int outlineThickness) {
	this->outlineThickness = outlineThickness;
}

void FontLoader::SetCommonInfo(int fontHeight, int base, int scaleW, int scaleH, int pages, bool isPacked) {
	font_->fontHeight = fontHeight;
	font_->base = base;
	font_->scaleW = scaleW;
	font_->scaleH = scaleH;
	font_->pages.resize(pages);
	for( int n = 0; n < pages; n++ )
		font_->pages[n] = 0;

	if( isPacked && outlineThickness )
		font_->hasOutline = true;
}

void FontLoader::AddChar(int id, int x, int y, int w, int h, int xoffset, int yoffset, int xadvance, int page, int chnl) {
	// Convert to a 4 element vector
	// TODO: Does this depend on hardware? It probably does
	if     ( chnl == 1 ) chnl = 0x00010000;  // Blue channel
	else if( chnl == 2 ) chnl = 0x00000100;  // Green channel
	else if( chnl == 4 ) chnl = 0x00000001;  // Red channel
	else if( chnl == 8 ) chnl = 0x01000000;  // Alpha channel
	else chnl = 0;

	if( id >= 0 )	{
		SCharDescr *ch = new SCharDescr;
		ch->srcX = x;
		ch->srcY = y;
		ch->srcW = w;
		ch->srcH = h;
		ch->xOff = xoffset;
		ch->yOff = yoffset;
		ch->xAdv = xadvance;
		ch->page = page;
		ch->chnl = chnl;

		font_->chars.insert(std::map<int, SCharDescr*>::value_type(id, ch));
	}

	if( id == -1 )	{
		font_->defChar.srcX = x;
		font_->defChar.srcY = y;
		font_->defChar.srcW = w;
		font_->defChar.srcH = h;
		font_->defChar.xOff = xoffset;
		font_->defChar.yOff = yoffset;
		font_->defChar.xAdv = xadvance;
		font_->defChar.page = page;
		font_->defChar.chnl = chnl;
	}
}

void FontLoader::AddKerningPair(int first, int second, int amount) {
	if( first >= 0 && first < 256 && font_->chars[first] )	{
		font_->chars[first]->kerningPairs.push_back(second);
		font_->chars[first]->kerningPairs.push_back(amount);
	}
}




FontLoaderBinaryFormat::FontLoaderBinaryFormat(FILE *f, const char *fontFile) : FontLoader(f, fontFile) {
}

int FontLoaderBinaryFormat::Load() {

 this->font_ = new Font();

	// Read and validate the tag. It should be 66, 77, 70, 2, 
	// or 'BMF' and 2 where the number is the file version.
	char magicString[4];
	fread(magicString, 4, 1, f);
	if( strncmp(magicString, "BMF\003", 4) != 0 )
	{
		//LOG(("Unrecognized format for '%s'", fontFile));
		fclose(f);
		return -1;
	}

	// Read each block
	char blockType;
	int blockSize;
	while( fread(&blockType, 1, 1, f) )	{
		// Read the blockSize
		fread(&blockSize, 4, 1, f);

		switch( blockType )	{
		case 1: // info
			ReadInfoBlock(blockSize);
			break;
		case 2: // common
			ReadCommonBlock(blockSize);
			break;
		case 3: // pages
			ReadPagesBlock(blockSize);
			break;
		case 4: // chars
			ReadCharsBlock(blockSize);
			break;
		case 5: // kerning pairs
			ReadKerningPairsBlock(blockSize);
			break;
		default:
			//LOG(("Unexpected block type (%d)", blockType));
			fclose(f);
			return -1;
		}
	}

	fclose(f);

	// Success
	return 0;
}

void FontLoaderBinaryFormat::ReadInfoBlock(int size) {
#pragma pack(push)
#pragma pack(1)
struct infoBlock {
    WORD fontSize;
    BYTE reserved:4;
    BYTE bold    :1;
    BYTE italic  :1;
    BYTE unicode :1;
    BYTE smooth  :1;
    BYTE charSet;
    WORD stretchH;
    BYTE aa;
    BYTE paddingUp;
    BYTE paddingRight;
    BYTE paddingDown;
    BYTE paddingLeft;
    BYTE spacingHoriz;
    BYTE spacingVert;
    BYTE outline;         // Added with version 2
    char fontName[1];
};
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);

	// We're only interested in the outline thickness
	infoBlock *blk = (infoBlock*)buffer;
	SetFontInfo(blk->outline);

	delete[] buffer;
}

void FontLoaderBinaryFormat::ReadCommonBlock(int size) {
#pragma pack(push)
#pragma pack(1)
struct commonBlock {
    WORD lineHeight;
    WORD base;
    WORD scaleW;
    WORD scaleH;
    WORD pages;
    BYTE packed  :1;
    BYTE reserved:7;
	BYTE alphaChnl;
	BYTE redChnl;
	BYTE greenChnl;
	BYTE blueChnl;
}; 
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);

	commonBlock *blk = (commonBlock*)buffer;

	SetCommonInfo(blk->lineHeight, blk->base, blk->scaleW, blk->scaleH, blk->pages, blk->packed ? true : false);

	delete[] buffer;
}

void FontLoaderBinaryFormat::ReadPagesBlock(int size) {
#pragma pack(push)
#pragma pack(1)
struct pagesBlock {
    char pageNames[1];
};
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);

	pagesBlock *blk = (pagesBlock*)buffer;

	for( int id = 0, pos = 0; pos < size; id++ ) {
		LoadPage(id, &blk->pageNames[pos], fontFile);
		pos += 1 + (int)strlen(&blk->pageNames[pos]);
	}

	delete[] buffer;
}

void FontLoaderBinaryFormat::ReadCharsBlock(int size) {
#pragma pack(push)
#pragma pack(1)
struct charsBlock {
    struct charInfo {
        DWORD id;
        WORD  x;
        WORD  y;
        WORD  width;
        WORD  height;
        short xoffset;
        short yoffset;
        short xadvance;
        BYTE  page;
        BYTE  chnl;
    } chars[1];
};
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);
	charsBlock *blk = (charsBlock*)buffer;

	for( int n = 0; int(n*sizeof(charsBlock::charInfo)) < size; n++ )	{
		AddChar(blk->chars[n].id,
		        blk->chars[n].x,
				blk->chars[n].y,
				blk->chars[n].width,
				blk->chars[n].height,
				blk->chars[n].xoffset,
				blk->chars[n].yoffset,
				blk->chars[n].xadvance,
				blk->chars[n].page,
				blk->chars[n].chnl);
	}

	delete[] buffer;
}

void FontLoaderBinaryFormat::ReadKerningPairsBlock(int size)
{
#pragma pack(push)
#pragma pack(1)
struct kerningPairsBlock {
    struct kerningPair  {
        DWORD first;
        DWORD second;
        short amount;
    } kerningPairs[1];
};
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);

	kerningPairsBlock *blk = (kerningPairsBlock*)buffer;

	for( int n = 0; int(n*sizeof(kerningPairsBlock::kerningPair)) < size; n++ )	{
		AddKerningPair(blk->kerningPairs[n].first,
		               blk->kerningPairs[n].second,
					   blk->kerningPairs[n].amount);
	}

	delete[] buffer;
}

} // end namespace

