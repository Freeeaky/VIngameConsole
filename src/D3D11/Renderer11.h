// =================================================================================
// Forward Declarations
// =================================================================================
class ID3D11Device;
class ID3D11DeviceContext;
class IFW1Factory;
class IFW1FontWrapper;

// =================================================================================
// D3D11 Renderer
// =================================================================================
class D3D11Renderer : public IRenderer
{
public:
	void Setup(void* pSetupData);
	void DrawText(int x, int y, Color col, string sText); // Default Font is FONT_SMALL
	void DrawText(eFonts eFont, int x, int y, Color col, string sText);

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	IFW1Factory* m_pFontFactory;
	IFW1FontWrapper* m_pFontWrapper;
};