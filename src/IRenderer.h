// =================================================================================
// Color
// =================================================================================
struct Color
{
	int r;
	int g;
	int b;
	int a;

	Color(int ar, int ag, int ab, int aa)
	{
		r = ar;
		g = ag;
		b = ab;
		a = aa;
	}
	UINT32 ToUINT32()
	{
		return ((UINT32)((((a)& 0xff) << 24) | (((b)& 0xff) << 16) | (((g)& 0xff) << 8) | ((r)& 0xff)));
	}
};

// =================================================================================
// Fonts
// =================================================================================
enum eFonts
{
	FONT_SMALL,
	FONT_NORMAL,
	FONT_BIG,
	_FONT_COUNT
};
static int FontSizes[_FONT_COUNT] = {
	12,
	18,
	25
};

// =================================================================================
// Renderer 
// =================================================================================
class IRenderer
{
public:
	virtual void Setup(void* pSetupData) = 0;
	virtual void DrawText(int x, int y, Color col, string sText) = 0; // Default Font is FONT_SMALL
	virtual void DrawText(eFonts eFont, int x, int y, Color col, string sText) = 0;
};

extern IRenderer* g_pRenderer;

// =================================================================================
// Renderer
// =================================================================================
#include "D3D11/Renderer11.h"