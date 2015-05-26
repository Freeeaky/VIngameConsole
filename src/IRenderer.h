// =================================================================================
// Color
// =================================================================================
struct Color
{
	__int8 r;
	__int8 g;
	__int8 b;

	Color(__int8 ar, __int8 ag, __int8 ab)
	{
		r = ar;
		g = ag;
		b = ab;
	}
};

// =================================================================================
// Fonts
// =================================================================================
enum eFonts
{
	FONT_SMALL,
	FONT_NORMAL,
	FONT_BIG
};

// =================================================================================
// Renderer 
// =================================================================================
class IRenderer
{
public:
	virtual void Setup(void* pSetupData);
	virtual void DrawText(int x, int y, Color col, char* sText) = 0; // Default Font is FONT_SMALL
	virtual void DrawText(eFonts eFont, int x, int y, Color col, char* sText) = 0;
};

extern IRenderer* g_pRender;

// =================================================================================
// Renderer
// =================================================================================
#include "D3D11/Renderer.h"