// =================================================================================
// Forward Declarations
// =================================================================================
class IDirect3DDevice9;
typedef struct IDirect3DDevice9 *LPDIRECT3DDEVICE9, *PDIRECT3DDEVICE9;
class CD3DFont;
class CD3DRender;

// =================================================================================
// D3D9 Renderer
// =================================================================================
class D3D9Renderer : public IRenderer
{
public:
	void Setup(void* pSetupData);
	void DrawText(int x, int y, Color col, string sText); // Default Font is FONT_SMALL
	void DrawText(eFonts eFont, int x, int y, Color col, string sText);

private:
	LPDIRECT3DDEVICE9 m_pDevice;
	CD3DFont* m_pFonts[_FONT_COUNT];
};